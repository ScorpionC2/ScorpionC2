//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "src/tests/engine/main.h"
#include "src/core/app/cli/colors/main.h"
#include "src/core/app/cli/logs/main.h"
#include "src/core/infra/sched/main.h"
#include "src/core/shared/types/main.h"
#include "src/core/shared/utils/random/main.h"
#include "src/tests/engine/unit/main.h"

#include <inttypes.h>
#include <stdint.h>
#include <time.h>

void initTests() {
    Scheduler = _initSched(Scheduler);

    Random.seed((int)time(NULL));
    Random.seed((int)Random.rand());
}

void finishTests() {
    _exitSched(Scheduler, FALSE);
    Logger.fmt.debugf(_DPSPEC "Successfully finished\n", _DPDATA);
}

bool_t run(void) {
    Logger.newLine.infoln(FG_WHITE_ITALIC "Starting" RESET " " FG_CYAN_ITALIC
                                          "Unit Tests" RESET);

    uint32_t unitAccumulator = 0;
    runUnitTests(&unitAccumulator);
    Logger.fmt.infof(FG_CYAN_ITALIC "%" PRIu32 FG_WHITE_ITALIC
                                    " Unit Tests failed" RESET "\n",
                     unitAccumulator);

    return unitAccumulator == 0 ? TRUE : FALSE;
}

int main(void) {
    initTests();

    Logger.newLine.infoln(FG_WHITE_ITALIC "Starting Tests" RESET);
    bool_t res = run() == FALSE;

    waitTasks(Scheduler);
    finishTests();

    Logger.newLine.infoln(FG_WHITE_ITALIC "Finished Tests" RESET);
    return res;
}
