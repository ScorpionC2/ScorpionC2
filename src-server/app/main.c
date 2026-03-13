//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "../shared/types/main.h"
#include "../shared/utils/random/main.h"
#include "../infra/hash/main.h"
#include "../infra/hash/hash_test.h"
#include "cli/logs/main.h"
#include "cli/colors/main.h"

#include <time.h>
#include <stdlib.h>
#include <string.h>

/*
 * TODO List:
 *  - Finish tests
 *  - Write a issue-solved
 *  - Remove the hash_test.h things
 *  - Remove '-lm' from make test target
 */

void init() {
    Random.seed(time(NULL));
    Random.seed(Random.rand());

}

void scorpion_wrapper(const void *data, size_t len, uint64_t seed, void *out) {
    bytes_t src = {
        .b = (uint8_t*)data,
        .len = len

    };

    scorpionSettings *old = Hash.settings;

    scorpionSettings s = {
        .seed = seed,
        .hashSize = small,
        .shiftSeed = seed & 0x1F

    };

    Hash.settings = &s;
    bytes_t h = Hash.ScorpionX(src);
    Hash.settings = old;

    memcpy(out,h.b,128);
    free(h.b);

}

int main() {
    init();
    hash_test_ctx ctx = {
        .func = scorpion_wrapper,
        .hash_size = 128,
        .name = "ScorpionX"

    };

    ht_run_all(&ctx);

    return 0;
    
}

