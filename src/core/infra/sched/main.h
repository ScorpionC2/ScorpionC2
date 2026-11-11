//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "main.h"
#include "src/core/infra/sched/context/main.h"
#include "src/core/infra/sched/processor/main.h"

#include <stdatomic.h>
#include <stdint.h>
#include <threads.h>

/**
 * TODO:
 * 1. Refactor `#include "main.h"` across the source to follow the project spec `"src/.../.../main.h"`;
 * 2. Doc functions.
 */

/**
 * INVESTIGATION NOTES:
 *                      There is two main execution flows that leads for infinity loops:
 *                          1st: stoppedMu DO NOT broadcasted;
 *                          2nd: Some other thing after stoppedMu;
 *                          3rd: I just found a 3rd state, rarer than the others, it's a memleak. I found it when debugging with gdb, so idk where the leak is yet;
 *                      
 *                      Yupiii, I just solved 1st one by locking/unlocking the stoppedMu before and after cnd_wait and cnd_broadcast.
 *                      Fixed 1st and 2nd by doing the same thing: adding mutexes. One good point is: I needed to lock/unlock the runnableMu (processors) inside a locked stoppedMu.
 *                      I don't find another memleak, so I'll ignore this.
 */

/**
 * NOTE:
 *      There is some problems that I need to fix: The 1st one is about how we create the stack memory, if we already mmap 512KiB
 *      of stack, why the heck we need to commit everything everytime. Require deep analysis.
 */

/**
 * @brief The scheduler main instance.
 * @note The core provides a global instance, however it still
 *       need to be initialized by _initSched().
 *       The recommended approach is creating a initializing
 *       function called in int main() before anything else.
 */
struct Sched_t {
    /**
     * @brief The global array of processors, holding every usable instance.
     * @note The MAX_SCHED_PROCESSORS can be changed at compile-time using -DMAX_SCHED_PROCESSORS <integer>.
     */
    struct Sched_Processor_t *processors[MAX_SCHED_PROCESSORS];

    /**
     * @brief The global queue of runnable tasks.
     * @note The Sched_t initializer must free it correctly using _exitSched().
     */
    struct SchedQueue_t *queue;

    /**
     * @brief The queue mutex lock;
     *
     * @note The _initSched must initialize it;
     * @note The _exitSched must destroy it;
     */
    mtx_t queueMu;

    /**
     * @brief The thread identifier.
     *
     * @note The _initSched() must initialize it against _runSched().
     * @note The _exitSched() must cleanly exit the thread by using the bool_t running value.
     */
    thrd_t thread;

    /**
     * @brief The atomic boolean that controls the scheduler execution.
     * @note Initialized as TRUE by _initSched.
     */
    atomic_bool running;

    /**
     * @brief Condition Variable for notifying _exitSched() when the _runSched() stop.
     * @note Must be initialized by _initSched().
     */
    cnd_t stopped;

    /**
     * @brief Mutex for the stopped cnd_t.
     *
     * @note Must be initialized by _initSched().
     * @note Used by _exitSched(), by the graceful shutdown logic.
     */
    mtx_t stoppedMu;

    /**
     * @brief Condition Variable for waking up the _runSched() thread when a new task arrives.
     * @note Must be initialized by _initSched().
     */
    cnd_t newTask;

    /**
     * @brief Mutex for the newTask cnd_t.
     * @note Must be initialized by _initSched().
     */
    mtx_t newTaskMu;

    /**
     * @brief Active tasks count, used for synchronizing (waiting for) tasks execution
     * @note Must be initialized by _initSched().
     */
    int64_t taskCount;

    /**
     * @brief Active tasks cnd_t, used to wait for every active task to stop.
     * @note Must be initialized by _initSched().
     */
    cnd_t active;

    /**
     * @brief Mutex for the active cnd_t.
     * @note Must be initialized by _initSched().
     */
    mtx_t activeMu;
};

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
 *
 * @pre @p sched being a valid pointer to a valid Sched_t instance.
 * @post @p sched unchanged.
 */
struct Sched_t *_initSched(struct Sched_t *sched);

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
int _runSched(void *sched);

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
void _exitSched(struct Sched_t *sched, bool_t graceful);

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
bool_t scorpionDo(struct Sched_t *sched, void (*func)(void *), void *arg);

/**
 * @brief Atomically decrement sched->taskCount.
 *
 * @p sched A pointer to the Sched_t instance that'll have it's taskCount decremented.
 *
 * @note Broadcasts @p sched->active cnd_t if taskCount reach 0.
 * @pre @p sched being a valid pointer to a valid Sched_t instance.
 */
void _decrementTasks(struct Sched_t *sched);

/**
 * @brief Atomically increment sched->taskCount.
 *
 * @p sched A pointer to the Sched_t instance that'll have it's taskCount incremented.
 * @pre @p sched being a valid pointer to a valid Sched_t instance.
 */
void _incrementTasks(struct Sched_t *sched);

/**
 * @brief Wait until all active tasks stop.
 *
 * @param sched A pointer to the Sched_t instance that'll have it's taskCount and active(Mu) used.
 * @pre @p sched being a valid pointer to a valid Sched_t instance.
 *
 * @note May block execution.
 */
void waitTasks(struct Sched_t *sched);

extern struct Sched_t *Scheduler;
