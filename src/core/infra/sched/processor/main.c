//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "src/core/infra/sched/processor/main.h"
#include "main.h"
#include "src/core/app/cli/logs/main.h"
#include "src/core/infra/sched/context/main.h"
#include "src/core/infra/sched/main.h"
#include "src/core/infra/sched/queue/main.h"
#include "src/core/infra/sched/tasks/main.h"

#include <stdint.h>
#include <stdlib.h>
#include <sys/select.h>
#include <threads.h>
#include <unistd.h>

/**
 * @brief Current processor pointer.
 *
 * This is a per-thread global processor instance, used by _exitTask() 
 * to swap back to the current processor and free the task.
 */
_Thread_local struct Sched_Processor_t *currentProcessor = NULL;

/**
 * @brief Initialize the processor.
 *
 * @param id The processor ID.
 * @param sched A pointer to the Scheduler.
 *
 * @reutrn Pointer to the new Processor.
 * @retval NULL Memory error.
 * @retval NULL Param fallback nullity.
 *
 * @note Caller owns the new Sched_Processor_t instance and must release it with _exitProcessor().
 */
struct Sched_Processor_t *_initProcessor(uint64_t id, void *sched) {
    if (sched == NULL)
        return NULL;

    struct Sched_Processor_t *out = malloc(sizeof(struct Sched_Processor_t));
    if (out == NULL)
        return NULL;

    out->id = id;
    out->context = (struct SchedContext_t){0x00};
    out->runnable = _initQueue();
    if (out->runnable == NULL)
        return NULL;

    out->current = NULL;
    out->currentRunning = FALSE;
    out->scheduler = sched;

    mtx_init(&out->runnableMu, mtx_plain);
    mtx_init(&out->stoppedMu, mtx_plain);

    cnd_init(&out->wake);
    cnd_init(&out->stopped);

    currentProcessor = out;
    return out;
}

/**
 * @brief Insert a new task to the processor's queue.
 *
 * @param processor A pointer to the processor that'll receive the new task.
 * @param task A pointer to the SchedTask_t instance that'll be
 *             appended to the @p processor->runnable queue.
 *
 * @return Pointer to task;
 * @retval NULL Param nullity fallback.
 * @retval NULL Memory Error.
 *
 * @warning Caller is responsible for locking @p processor->runnableMu.
 *
 * @pre @p task being a valid pointer to a valid SchedTask_t instance.
 * @pre @p processor being a valid pointer to a valid Sched_Processor_t instance.
 *
 * @post @p task unchanged.
 * @post @p processor unchanged.
 */
struct SchedTask_t *_insertTask(struct SchedTask_t *task,
                                struct Sched_Processor_t *processor) {
    if (task == NULL || processor == NULL)
        return NULL;

    struct SchedTask_t *t = _appendQueue(processor->runnable, task);
    if (t == NULL)
        return NULL;

    return task;
}

/**
 * @brief Get the next task.
 *
 * @param processor A pointer to the Sched_Processor_t instance that'll be used for task searching.
 *
 * @return Pointer to the next task.
 * @retval NULL Param nullity fallback.
 * @retval NULL @p processor->runnable is NULL.
 * @retval NULL Next task is null.
 *
 * @note Caller must upgrade @p processor->current.
 * @note This is a message from hoWo to all developers reading the core.
 *       If the function is just a bunch of nullity checks and a simple
 *       processor->runnable->next, why we need a function? It's just
 *       calling overhead without real useful features. So, this question
 *       is pretty simple, we need maintaiability and a decoupled architecture.
 *       For future implementations (round-robin or even I/O notifiers), we don't
 *       want to rewrite the caller (to this function), we want to just
 *       modify this and keep using it in the same way we're used to.
 *       A decoupled function let margin for future upgrades and algorithm
 *       rewrites, so we keep it simple now and then we upgrade it to the
 *       next level, as seen in other sucessfull projects!
 *
 * @warning Caller is responsible for locking @p processor->runnableMu.
 *
 * @pre @p processor being a valid pointer to a valid Sched_Processor_t instance.
 * @post Nothing changed.
 */
struct SchedTask_t *_nextTask(struct Sched_Processor_t *processor) {
    if (processor == NULL)
        return NULL;

    if (processor->runnable == NULL)
        return NULL;

    if (_setNextCursor(processor->runnable) == NULL)
        return NULL;

    struct SchedTask_t *next = processor->runnable->cursor->task;
    return next;
}

/**
 * @brief Run a given Sched_Processor_t.
 *
 * @param processor A pointer to the Sched_Processor_t instance to be run.
 *
 * @return Execution Success.
 * @retval 0 Successfully executed.
 * @retval -1 Param nullity fallback.
 * @retval -1 Can't create the runnable queue.
 * @retval -1 Can't insert the @p firstTask.
 *
 * @note The per-thread global Sched_Processor_t instance (a.k.a. currentProcessor) will be set as
 *       the @p processor parameter.
 *
 * @pre @p processor and @p firstTask both being valid pointers to valid instances of itself's structs.
 */
int _runProcessor(void *arg) {
    if (arg == NULL)
        return FALSE;

    struct Sched_Processor_t *processor = (struct Sched_Processor_t *)(arg);
    currentProcessor = processor;

    processor->currentRunning = TRUE;
    while (TRUE) {
        // Save processor ctx
        _context_get_linux_amd64(&processor->context);
        processor->current = processor->runnable->cursor
                                 ? processor->runnable->cursor->task
                                 : NULL;

        // Mutex-containerized accesses
        mtx_lock(&processor->runnableMu);

        // If cursor = NULL
        while (processor->runnable->cursor == NULL &&
               processor->currentRunning == TRUE) {
            if (processor->runnable->head != NULL) {
                processor->runnable->cursor = processor->runnable->head;
                processor->current = processor->runnable->cursor->task;
                break;
            }

            cnd_wait(&processor->wake, &processor->runnableMu);
        }

        if (processor->currentRunning == FALSE) {
            mtx_unlock(&processor->runnableMu);
            break;
        }

        // If dead (returning from _exitTask()), clean stack
        if (processor->runnable->cursor->prev &&
            processor->runnable->cursor->prev->task->state == DEAD) {
            _cleanStack(&processor->runnable->cursor->prev->task->stack);
            _decrementTasks((struct Sched_t *)(processor->scheduler));
        }

        mtx_unlock(&processor->runnableMu);

        // Swap to task.
        if (processor->current->state != DEAD)
            _context_swap_linux_amd64(&processor->context,
                                      &processor->current->ctx);

        mtx_lock(&processor->runnableMu);

        // Set next cursor and check
        struct SchedTask_t *t = _setNextCursor(currentProcessor->runnable);
        if (t == NULL) {
            mtx_unlock(&processor->runnableMu);
            continue;
        }

        // Keep running
        processor->current = processor->runnable->cursor->task;
        mtx_unlock(&processor->runnableMu);
    }

    mtx_lock(&processor->stoppedMu);
    cnd_broadcast(&processor->stopped);
    mtx_unlock(&processor->stoppedMu);

    return TRUE;
}

/**
 * @brief Cleanly exits the processor.
 *
 * @param processor A pointer to the Sched_Processor_t instance that'll be cleaned.
 * @note This implementation ensure that the current running task will be finished.
 *
 * @pre @p processor being a valid pointer to a valid Sched_Processor_t instance.
 */
void _exitProcessor(struct Sched_Processor_t *processor) {
    if (processor == NULL)
        return;

    processor->currentRunning = FALSE;
    mtx_lock(&processor->stoppedMu);

    mtx_lock(&processor->runnableMu);
    cnd_broadcast(&processor->wake);
    mtx_unlock(&processor->runnableMu);

    cnd_wait(&processor->stopped, &processor->stoppedMu);
    mtx_unlock(&processor->stoppedMu);

    mtx_lock(&processor->runnableMu);
    if (processor->runnable->head != NULL &&
        processor->runnable->tail != NULL) {
        processor->runnable->cursor = processor->runnable->head;
        struct SchedQueue_Node_t *cur = processor->runnable->cursor;

        while (cur != NULL) {
            struct SchedQueue_Node_t *next = cur->next;
            _deleteNode(cur, processor->runnable);

            cur = next;
        }
    }

    free(processor->runnable);

    mtx_unlock(&processor->runnableMu);
    mtx_destroy(&processor->runnableMu);
    mtx_destroy(&processor->stoppedMu);

    cnd_destroy(&processor->wake);
    cnd_destroy(&processor->stopped);

    free(processor);
}
