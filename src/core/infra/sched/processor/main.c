//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "src/core/infra/sched/processor/main.h"
#include "infra/sched/context/main.h"
#include "infra/sched/queue/main.h"
#include "infra/sched/tasks/main.h"
#include "main.h"
#include <stdlib.h>
#include <sys/select.h>
#include <threads.h>

/**
 * @brief Current processor pointer.
 *
 * This is a per-thread global processor instance, used by _exitTask() 
 * to swap back to the current processor and free the task.
 */
_Thread_local struct Sched_Processor_t *currentProcessor = NULL;

/**
 * @brief CPU-Lazy Spin Sleeping.
 */
void spinSleep(uint64_t usecs) {
    struct timeval timeout = {.tv_sec = usecs / 1000000,
                              .tv_usec = usecs % 1000000};

    select(0, NULL, NULL, NULL, &timeout);
}

// TODO: Initialize mtx_t mu & cnd_t wake.
/**
 * @brief Initialize the processor.
 *
 * @reutrn Pointer to the new Processor.
 * @retval NULL Memory error.
 *
 * @note Caller owns the new Sched_Processor_t instance and must release it with _exitProcessor().
 */
struct Sched_Processor_t *_initProcessor() {
    struct Sched_Processor_t *out = malloc(sizeof(struct Sched_Processor_t));
    if (out == NULL)
        return NULL;

    out->context = (struct SchedContext_t){0x00000000};
    out->runnable = _initQueue();
    if (out->runnable == NULL)
        return NULL;

    out->current = out->runnable->cursor->task;
    out->currentIsRunning = FALSE;
    out->currentRunning = FALSE;

    currentProcessor = out;
    return out;
}

/**
 * @brief Insert a new task to the processor's queue.
 *
 * @param task A pointer to the SchedTask_t instance that'll be appended
 *             to the runnable queue.
 *
 * @return Pointer to task;
 * @retval NULL Param nullity fallback.
 * @retval NULL Memory Error.
 *
 * @pre @p task being a valid pointer to a valid SchedTask_t instance.
 * @post @p task unchanged.
 */
struct SchedTask_t *_insertTask(struct SchedTask_t *task) {
    if (task == NULL || currentProcessor == NULL)
        return NULL;

    struct SchedTask_t *t = _appendQueue(currentProcessor->runnable, task);
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

// TODO: Use the mtx_t mu.
// TODO: Use the cnd_t wake instead of spinSleep.
/**
 * @brief Run a given Sched_Processor_t.
 *
 * @param processor A pointer to the Sched_Processor_t instance to be run.
 * @param firstTask A pointer to the SchedTask_t instance that'll be the first task executed.
 *
 * @return Execution Success (boolean).
 * @retval FALSE Param nullity fallback.
 * @retval FALSE Can't create the runnable queue.
 * @retval FALSE Can't insert the @p firstTask.
 *
 * @note The per-thread global Sched_Processor_t instance (a.k.a. currentProcessor) will be set as
 *       the @p processor parameter.
 *
 * @pre @p processor and @p firstTask both being valid pointers to valid instances of itself's structs.
 */
bool_t _runProcessor(struct Sched_Processor_t *processor) {
    if (processor == NULL)
        return FALSE;

    currentProcessor = processor;

    processor->runnable->cursor = processor->runnable->head;
    processor->current = processor->runnable->cursor->task;

    processor->currentRunning = TRUE;
    while (processor->currentRunning) {
        _context_get_linux_amd64(&processor->current->ctx);
        if (processor->runnable->cursor->prev->task->state == DEAD)
            _cleanStack(&processor->runnable->cursor->prev->task->stack);

        if (processor->current->state != DEAD)
            _context_set_linux_amd64(&processor->current->ctx);

        while (currentProcessor->runnable->cursor->next == NULL)
            spinSleep(250);

        struct SchedTask_t *t = _setNextCursor(currentProcessor->runnable);
        if (t == NULL)
            return FALSE;

        _deleteNode(currentProcessor->runnable->cursor->prev);
        processor->current = processor->runnable->cursor->task;

        if (processor->currentRunning == FALSE) {
            processor->currentIsRunning = processor->currentRunning;
            break;
        }
    }

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
    while (processor->currentIsRunning)
        spinSleep(750);

    // Considering that @p processor->currentIsRunning changed to FALSE.
    if (processor->runnable->head != NULL &&
        processor->runnable->tail != NULL) {
        processor->runnable->cursor = processor->runnable->head;

        struct SchedQueue_Node_t *cur = processor->runnable->cursor;
        while (cur != NULL) {
            struct SchedQueue_Node_t *next = cur->next;
            _deleteNode(cur);
            cur = next;
        }
    }

    free(processor->runnable);
    free(processor);
}
