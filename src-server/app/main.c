//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "src-server/shared/types/main.h"
#include "src-server/shared/utils/random/main.h"

#include <time.h>

/*
 * TODO List:
 *  - Implement /src-server/infra/fs
 *  - Study networking to start implementing /src-server/infra/socket
 *
 */

void init() {
    Random.seed((int)time(NULL));
    Random.seed((int)Random.rand());
}

int main() {
    init();
    return 0;
}
