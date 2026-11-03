//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "src/core/infra/sched/main.h"
#include "src/core/app/cli/logs/main.h"
#include <stdlib.h>

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

    sched->queue = NULL;
    for (int i = 0; i < MAX_SCHED_PROCESSORS; i++) {
        sched->processors[i] = _initProcessor();
    }

    sched->running = ATOMIC_VAR_INIT(true);
    thrd_create(&sched->thread, _runSched, sched);
    return sched;
};

void scorpionDo(struct Sched_t *self, void (*func)(void *), void *arg);
