//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "src/core/infra/sched/main.h"
#include "src/core/app/cli/colors/main.h"
#include "src/core/app/cli/logs/main.h"
#include "src/core/infra/sched/processor/main.h"
#include "src/core/infra/sched/tasks/main.h"
#include "src/core/shared/types/main.h"

#include <stdatomic.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

struct Sched_t *Scheduler = NULL;

/**
 * @brief Initialize a struct Sched_t instance.
 * @param sched A pointer to the Sched_t instance to be initialized.
 *
 * @return The @p sched pointer.
 * @retval NULL Param nullity fallback.
 *
 * @note The function WON'T initialize the @p sched->queue as it needs an initial task.
 * @note The function WON'T create the processors threads as each one needs an initial task.
 * @note The @p sched is owned by caller and it's memory must be released using _exitSched().
 * @pre @p sched being a valid pointer to a valid Sched_t instance.
 *
 * @post @p sched unchanged.
 */
struct Sched_t *_initSched(struct Sched_t *sched) {
    // Prevent double initializing.
    if (sched != NULL)
        return NULL;

    sched = malloc(sizeof(struct Sched_t));
    if (sched == NULL) {
        // The scheduler nullity is a critical error and must be treated so.
        // Exiting prevents errors throughout the software.
        // TODO: Create a critical allocation function that fallback to memory arenas if malloc fails.
        Logger.newLine.errorln("Can't allocate memory to the global Sched_t "
                               "instance (scheduler): Exiting...");
        exit(1);
    }

    sched->queue = _initQueue();
    sched->taskCount = 0;

    cnd_init(&sched->stopped);
    cnd_init(&sched->newTask);
    cnd_init(&sched->active);

    mtx_init(&sched->stoppedMu, mtx_plain);
    mtx_init(&sched->newTaskMu, mtx_plain);
    mtx_init(&sched->queueMu, mtx_plain);
    mtx_init(&sched->activeMu, mtx_plain);

    for (int i = 0; i < MAX_SCHED_PROCESSORS; i++) {
        sched->processors[i] = _initProcessor(i + 1, sched);
        thrd_create(&sched->processors[i]->thread, _runProcessor,
                    sched->processors[i]);
    }

    atomic_init(&sched->running, true);
    thrd_create(&sched->thread, _runSched, sched);
    return sched;
};

/**
 * @brief Run the scheduler.
 *
 * @param sched A pointer to the Sched_t instance that'll run.
 *
 * @return Exection status.
 * @retval 0 Successfully executed.
 * @retval -1 Param nullity fallback.
 *
 * @pre @p sched being a valid pointer to a valid Sched_t instance.
 *
 * WHILE Workflow:
 *       1. Check head, if NULL cnd_wait on newTask;
 *       2. Check sched->running, if FALSE it breaks the loop;
 *       3. mtx_lock on [iter]->runnableMu;
 *       4. Insert new task on [iter]->runnable;
 *       5. Remove task node from local queue, pointing it to rnode;
 *       6. Free rnode instance;
 *       7. cnd_signal [iter]->wake;
 *       8. mtx_unlock [iter]->runnableMu;
 *       9. iter++ cap MAX_SCHED_PROCESSORS;
 */
int _runSched(void *arg) {
    if (arg == NULL)
        return -1;

    struct Sched_t *sched = (struct Sched_t *)(arg);
    size_t iter = 0;

    while (TRUE) {
        while (sched->queue->head == NULL &&
               atomic_load(&sched->running) == TRUE)
            cnd_wait(&sched->newTask, &sched->newTaskMu);

        if (atomic_load(&sched->running) == FALSE)
            break;

        mtx_lock(&sched->processors[iter % MAX_SCHED_PROCESSORS]->runnableMu);

        _insertTask(sched->queue->head->task,
                    sched->processors[iter % MAX_SCHED_PROCESSORS]);
        struct SchedQueue_Node_t *rnode =
            _removeNode(sched->queue->head, sched->queue);

        free(rnode);

        cnd_signal(&sched->processors[iter % MAX_SCHED_PROCESSORS]->wake);
        mtx_unlock(&sched->processors[iter % MAX_SCHED_PROCESSORS]->runnableMu);

        iter = (iter + 1) % MAX_SCHED_PROCESSORS;
    }

    mtx_lock(&sched->stoppedMu);
    cnd_broadcast(&sched->stopped);
    mtx_unlock(&sched->stoppedMu);

    return 0;
}

/**
 * @brief Exit the scheduler.
 *
 * @param sched A pointer to the Sched_t instance that'll be finished.
 * @param graceful Decides if the exiting process will wait for every task to finish.
 *
 * @warning You praobably do not want to exit the scheduler mid-run your app, think a bit before calling this function.
 *
 * @pre @p sched being a valid pointer to a valid Sched_t instance.
 * @post @p sched and it's child resources released.
 */
void _exitSched(struct Sched_t *sched, bool_t graceful) {
    if (sched == NULL)
        return;

    if (graceful)
        waitTasks(sched);

    mtx_lock(&sched->stoppedMu);
    atomic_store(&sched->running, FALSE);

    mtx_lock(&sched->newTaskMu);
    cnd_broadcast(&sched->newTask);
    mtx_unlock(&sched->newTaskMu);

    cnd_wait(&sched->stopped, &sched->stoppedMu);
    mtx_unlock(&sched->stoppedMu);

    for (int i = 0; i < MAX_SCHED_PROCESSORS; i++)
        _exitProcessor(sched->processors[i]);

    if (sched->queue->head != NULL && sched->queue->tail != NULL) {
        sched->queue->cursor = sched->queue->head;
        struct SchedQueue_Node_t *cur = sched->queue->cursor;

        while (cur != NULL) {
            struct SchedQueue_Node_t *next = cur->next;
            _deleteNode(cur, sched->queue);
            cur = next;
        }
    }

    free(sched->queue);

    mtx_destroy(&sched->newTaskMu);
    mtx_destroy(&sched->stoppedMu);
    mtx_destroy(&sched->queueMu);
    mtx_destroy(&sched->activeMu);

    cnd_destroy(&sched->stopped);
    cnd_destroy(&sched->newTask);
    cnd_destroy(&sched->active);

    free(sched);
}

/**
 * @brief Add a new task to the Sched_t->queue.
 *
 * @param sched A pointer to the scheduler (Sched_t) instance that'll receive the new task.
 * @param func The task function.
 * @param arg The task argument, nullable by design.
 *
 * @return Execution status.
 * @retval FALSE Null Sched_t *sched argument received.
 * @retval FALSE Can't initialize the task.
 * @retval FALSE Can't append the @p sched->queue.
 *
 * @pre @p sched being a valid pointer to a valid Sched_t instance.
 * @pre @p func being a valid function.
 * @pre @p arg being a valid pointer to any data type OR NULL.
 *
 * @post The task created and inserted in @p sched->queue tasks queue.
 */
bool_t scorpionDo(struct Sched_t *sched, void (*func)(void *), void *arg) {
    // Do not check func and arg, they're processor checked // initTask checked.
    if (sched == NULL)
        return FALSE;

    struct SchedTask_t *task = _initTask(func, arg);
    if (task == NULL)
        return FALSE;

    mtx_lock(&sched->queueMu);
    mtx_lock(&sched->newTaskMu);

    if (_appendQueue(sched->queue, task) == NULL)
        return FALSE;

    _incrementTasks(sched);
    cnd_signal(&sched->newTask);

    mtx_unlock(&sched->newTaskMu);
    mtx_unlock(&sched->queueMu);

    return TRUE;
}

/**
 * @brief Wait until all active tasks stop.
 *
 * @param sched A pointer to the Sched_t instance that'll have it's taskCount and active(Mu) used.
 * @pre @p sched being a valid pointer to a valid Sched_t instance.
 *
 * @note May block execution.
 */
void waitTasks(struct Sched_t *sched) {
    if (sched == NULL)
        return;

    mtx_lock(&sched->activeMu);

    while (sched->taskCount > 0)
        cnd_wait(&sched->active, &sched->activeMu);

    mtx_unlock(&sched->activeMu);
}

/**
 * @brief Atomically decrement sched->taskCount.
 *
 * @p sched A pointer to the Sched_t instance that'll have it's taskCount decremented.
 *
 * @note Broadcasts @p sched->active cnd_t if taskCount reach 0.
 * @pre @p sched being a valid pointer to a valid Sched_t instance.
 */
void _decrementTasks(struct Sched_t *sched) {
    if (sched == NULL)
        return;

    mtx_lock(&sched->activeMu);
    if (--sched->taskCount == 0) {
        cnd_broadcast(&sched->active);
        mtx_unlock(&sched->activeMu);

        return;
    }

    mtx_unlock(&sched->activeMu);
}

/**
 * @brief Atomically increment sched->taskCount.
 *
 * @p sched A pointer to the Sched_t instance that'll have it's taskCount incremented.
 * @pre @p sched being a valid pointer to a valid Sched_t instance.
 */
void _incrementTasks(struct Sched_t *sched) {
    if (sched == NULL)
        return;

    mtx_lock(&sched->activeMu);
    sched->taskCount++;
    mtx_unlock(&sched->activeMu);
}
