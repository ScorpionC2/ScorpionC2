//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "src/core/infra/sched/context/main.h"
#include "src/core/infra/sched/processor/main.h"

#include <stdatomic.h>
#include <stdint.h>
#include <threads.h>

/**
 * TODO:
 *      1. _initSched implementation that creates the main thread running the void _runSched();
 *      2. _initProcessor must initialize it's mutexes;
 *      3. _initSched must initialize the processor threads;
 *      4. I need to create _exitSched();
 *      5. Need some way to cleanly exit threads, I can easily control this for processor threads and
 *         scheduler threads but how do I exit a running task without context control? Need analysis.
 *  
 * Stuff:
 *      - The _runSched() will be a dispatcher that atomically dispatches tasks from the Scheduler->queue to the processor threads.
 *  
 * Fix and Improvements:
 *      1. The processor dispatcher MUST use mutexes to containerize queue->cursor accesses
 */

/**
 * NOTE:
 *      1. The scheduler dispatcher must reallocate tasks from the Sched_t->queue to the Sched_Processor_t->queue;
 *      2. The task reallocation must check for ID re-use.
 *      3. The _runSched dispatcher must check for queue length for each processor;
 *      4. The _initSched will NOT fully initialize every processor and it's threads as it needs a initial task.
 *      5. The _initSched will NOT initialize the Sched_t->queue as it needs a initial task.
 */

/**
 * MORE NOTES:
 *      There is some problems that I need to fix: The 1st one is about how we create the stack memory, if we allocate 512KiB
 *      of stack, why the heck we need to commit everything everytime. Require deep analysis.
 *       
 *      How will we solve the NOTE->4 and NOTE->5 issues? I don't know yet but we might need a check control flow branch in the scorpionDo function.
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
     * @brief The thread identifier.
     *
     * @note The _initSched() must initialize it against _runSched().
     * @note The _exitSched() must cleanly exit the thread by using the bool_t running value.
     */
    thrd_t thread;

    /**
     * 
     */
    mtx_t procMu;

    /**
     * @brief The atomic boolean that controls the scheduler execution.
     * @note Initialized as TRUE by _initSched.
     */
    atomic_bool running;
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
int _runSched(void *sched);
void _exitSched(struct Sched_t *sched);

void scorpionDo(struct Sched_t *self, void (*func)(void *), void *arg);

extern struct Sched_t *Scheduler;
