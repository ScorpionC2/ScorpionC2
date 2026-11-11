//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "src/core/app/cli/logs/main.h"
#include "src/core/infra/sched/main.h"
#include "src/core/shared/utils/random/main.h"

void init() {
    Scheduler = _initSched(Scheduler);

    Random.seed((int)time(NULL));
    Random.seed((int)Random.rand());
}

void finish() {
    _exitSched(Scheduler, FALSE);
    Logger.fmt.debugf(_DPSPEC "Successfully exited "
                              "Global Scheduler\n",
                      _DPDATA);
}

int main() {
    init();
    finish();

    return 0;
}
