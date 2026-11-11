//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "shared/utils/random/main.h"
#include "src/core/infra/sched/main.h"
#include "src/core/shared/types/main.h"
#include "src/core/shared/utils/math/main.h"
#include "tests/engine/unit/main.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void foo(void *) {
    volatile uint64_t sink = Random.rand();
    for (volatile int i = 0; i < 1000000000; i++)
        sink ^= i * rotr64(sink, i);
}

bool_t task_base(void) { return scorpionDo(Scheduler, foo, NULL); }

bool_t task_much(void) {
    const size_t tasksCount = 16;
    bool_t fail = FALSE;

    for (size_t i = 0; i < tasksCount; i++)
        fail |= !scorpionDo(Scheduler, foo, NULL);

    return !fail;
}

TESTS_UNIT_REGISTER(task_base, "Scheduler task running 1BI operations");
TESTS_UNIT_REGISTER(task_much,
                    "Scheduler multiple tasks running 1BI operations")
