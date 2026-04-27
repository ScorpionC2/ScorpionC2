//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "tests/engine/main.h"
#include "src-server/app/cli/colors/main.h"
#include "src-server/app/cli/logs/main.h"
#include "src-server/shared/types/main.h"
#include "tests/engine/unit/main.h"
#include <inttypes.h>
#include <stdint.h>

bool_t run(void) {
    Logger.infoln(FG_WHITE_ITALIC "Starting" RESET " " FG_CYAN_ITALIC
                                  "Unit Tests" RESET);

    uint32_t unitAccumulator = 0;
    runUnitTests(&unitAccumulator);
    Logger.infof(FG_CYAN_ITALIC "%" PRIu32 FG_WHITE_ITALIC
                                " Unit Tests failed" RESET "\n",
                 unitAccumulator);

    return unitAccumulator == 0 ? TRUE : FALSE;
}

int main(void) {
    Logger.infoln(FG_WHITE_ITALIC "Starting Tests" RESET);
    return run() == FALSE;
}
