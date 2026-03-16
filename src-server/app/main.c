//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "../shared/types/main.h"
#include "../shared/utils/random/main.h"

#include <time.h>

/*
 * TODO List:
 *  - Implement /src-server/infra/fs
 *
 */

void init() {
    Random.seed(time(NULL));
    Random.seed(Random.rand());

}

int main() {
    init();
    return 0;
    
}

