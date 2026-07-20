//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//
// This file uses the formula presented by:
//  https://arxiv.org/abs/1402.6246
//  https://en.wikipedia.org/wiki/Xorshift
//  https://www.youtube.com/watch?v=XDsYPXRCXAs
//  https://www.ams.org/journals/mcom/1999-68-225/S0025-5718-99-00996-5/S0025-5718-99-00996-5.pdf
// This file contains upgraded versions of that formula
//
// Last updates and fixes was based in:
//  https://romailler.ch/2020/07/28/crypto-modulo_bias_guide/?utm_source=reddit.com
//  https://stackoverflow.com/questions/10984974/why-do-people-say-there-is-modulo-bias-when-using-a-random-number-generator?utm_source=google.com
//  https://www.pcg-random.org/posts/xoroshiro-fails-truncated.html?utm_source=reddit.com
//  https://en.wikipedia.org/wiki/Random_number_generation
//  https://en.wikipedia.org/wiki/Randomness_extractor?utm_source=google.com
//
// ps: removed the large "docs" because it's useless
//
// Last upgrades (aka rdseed and rdrand use) is very inspired by my friend's video: https://youtu.be/fyBr9iKZrZo?si=eBSlXBXzTRLL4isi
//

#include "src/core/shared/utils/random/main.h"

#include <immintrin.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

static uint64_t seed_g = 0;

void Randomseed(uint64_t seed) { seed_g = seed; }

uint64_t Randomrand() {
    seed_g = (84971284 + seed_g * 1664525);
    uint64_t s = (seed_g << 9) ^ (seed_g >> 30);

    s ^= s >> 37;
    s *= 0xbf58476d1ce4e5b9ULL;
    s ^= s >> 19;

    return s;
};

uint64_t Randomrandr(int min, int max) {
    uint32_t range = (uint64_t)(max - min + 1);
    uint32_t x;
    uint32_t limit = UINT64_MAX - (UINT64_MAX % range);

    do {
        x = Randomrand();

    } while (x >= limit);

    return (x & (range - 1)) + min;
}

const RandomInstance Random = {
    .seed = Randomseed, .rand = Randomrand, .randr = Randomrandr

};
