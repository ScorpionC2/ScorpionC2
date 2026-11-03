//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "main.h"
#include "src/core/infra/sched/context/main.h"
#include "src/core/infra/sched/queue/main.h"
#include "src/core/infra/sched/tasks/main.h"

#include <stdint.h>
#include <threads.h>

#ifndef MAX_SCHED_PROCESSORS
#define MAX_SCHED_PROCESSORS 4
#endif

/**
 * @brief Scheduler [P]rocessor.
 *
 * The processor is the thread that runs the tasks, by swapping itself's context.
 *
 * @note Don't initialize manually, use _initProcessor() instead.
 */
struct Sched_Processor_t {
    /**
     * @brief Sched_Processor_t context.
     */
    struct SchedContext_t context;

    /**
     * @brief Pointer to the SchedQueue_t that stores the runnable tasks queue.
     */
    struct SchedQueue_t *runnable;

    /**
     * @brief Current task.
     */
    struct SchedTask_t *current;

    /**
     * @brief Current processor running flag.
     *
     * This is a per-thread global processor running flag, used by _runProcessor()
     * to run and gracefully shutdown.
     */
    bool_t currentRunning;

    /**
     * @brief Current processor is running flag.
     * @note Do not confuse with currentRunning, that is used in _runProcessor() main
     *       loop. This one is used by _exitProcessor(), by the graceful shutdown, to
     *       ensure that the processor isn't running any task or loop/check.
     */
    bool_t currentIsRunning;

    /**
     * @brief Mutex lock to queue access.
     * @note Must be initialized by _initProcessor().
     */
    mtx_t mu;

    /**
     * @brief Condition variable that wakes up the processor when a new task arrives.
     * @note Must be initialized by _initProcessor().
     */
    cnd_t wake;
};

/**
 * @brief Initialize the processor.
 *
 * @reutrn Pointer to the new Processor.
 * @retval NULL Memory error.
 *
 * @note The processor is firstly created without a valid queue.
 *       User must add the task using _insertTask().
 *
 * @note Caller owns the new Sched_Processor_t instance and must release it with _exitProcessor().
 */
struct Sched_Processor_t *_initProcessor();

/**
 * @brief Insert a new task to the processor's queue.
 *
 * @param task A pointer to the SchedTask_t instance that'll be appended
 *             to the runnable queue.
 *
 * @return Pointer to task;
 * @retval NULL Param nullity fallback or Memory Error.
 *
 * @note If currentProcessor->runnable is NULL, the function creates it.
 *       This created queue is owned by caller, that must release it's
 *       memory when it is no longer needed, using _exitProcessor() wrapper.
 *
 * @pre @p task being a valid pointer to a valid SchedTask_t instance.
 * @post @p task unchanged.
 */
struct SchedTask_t *_insertTask(struct SchedTask_t *task);

/**
 * @brief Get the next task.
 *
 * @param processor A pointer to the Sched_Processor_t instance that'll be used for task searching.
 *
 * @return Pointer to the next task.
 * @retval NULL Param nullity fallback.
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
struct SchedTask_t *_nextTask(struct Sched_Processor_t *processor);

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
bool_t _runProcessor(struct Sched_Processor_t *processor,
                     struct SchedTask_t *firsttask);

/**
 * @brief Cleanly exits the processor.
 *
 * @param processor A pointer to the Sched_Processor_t instance that'll be cleaned.
 * @note This implementation ensure that the current running task will be finished.
 *
 * @pre @p processor being a valid pointer to a valid Sched_Processor_t instance.
 */
void _exitProcessor(struct Sched_Processor_t *processor);

/**
 * @brief Current processor pointer.
 *
 * This is a per-thread global processor instance, used by _exitTask()
 * to swap back to the current processor and free the task.
 */
extern _Thread_local struct Sched_Processor_t *currentProcessor;
