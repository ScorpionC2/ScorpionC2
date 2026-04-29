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

#include "src-server/shared/utils/random/main.h"
#include "src-server/shared/utils/math/main.h"
#include <immintrin.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#define _RDRAND_MAX_RETRIES 24
#define _RDSEED_MAX_RETRIES 24
#define CHECK(x, cap)                                                          \
    if ((x) > (cap))                                                           \
        break;                                                                 \
    else                                                                       \
        (x)++;

uint32_t seed_g = 1;

unsigned long long getRDRAND() {
    unsigned long long rdrand_val = 0;
    size_t count = 0;

    while (_rdrand64_step(&rdrand_val) == 0) {
        CHECK(count, _RDRAND_MAX_RETRIES)
    }

    return rdrand_val;
};

unsigned long long getRDSEED() {
    unsigned long long rdseed_val = 0;
    size_t count = 0;

    while (_rdseed64_step(&rdseed_val) == 0) {
        CHECK(count, _RDRAND_MAX_RETRIES)
    }

    return rdseed_val;
};

void Randomseed(int seed) { seed_g = seed; }

uint32_t Randomrseed(int seed) {
    uint32_t pid = (uint32_t)getpid();
    uint32_t gid = (uint32_t)getgid();
    uint32_t timeNow = time(NULL);
    unsigned long long rdseed = getRDSEED();

    uint32_t out = rdseed;
    out ^= rotl(out, pid);
    out *= gid;
    out ^= rotr(out, timeNow);
    out <<= rdseed & 0x1F;
    out ^= rotr(rdseed, (gid & 0x1F));

    seed_g = rdseed ^ out;

    return out;
}

uint32_t Randomrand() {
    uint32_t s = Randomrseed(seed_g ^ 0x6C656574);
    if (seed_g == 1) {
        uint32_t rdrand = getRDRAND();
        uint32_t rdseed = getRDSEED();

        seed_g = rdrand;
        seed_g ^= rdseed;
        seed_g *= 0xDEADBEEF;
        seed_g ^= rotl(seed_g, (rdseed & 0x1F));

        s = rotr(seed_g, 19);
        seed_g = Randomrseed(s);
    }

    s = ((s << 13) ^ s) - (s >> 21);

    uint32_t n = s;
    n = (n * (n * n * 15732 + 789221) + 771171059);
    n += s;
    n = ((n << 14) ^ n) - (n >> 20);

    uint32_t out;
    for (int i = 0; i < 8; i++) {
        uint32_t old = n;

        uint32_t a = n ^ 0x0539;
        uint32_t b = rotr(n, 15);
        uint32_t c = rotl(a, b) ^ 0x1337;

        a += b;
        c ^= a;
        c = rotl(c, 16);

        b += c;
        a ^= b;
        a = rotl(a, 12);

        out = a;
        out ^= b;
        out *= rotl(out, old);
        out ^= n;
        out *= rotr(out, 17);
        out ^= c;
    }

    return out;
};

uint32_t Randomrandr(int min, int max) {
    uint32_t range = (uint32_t)(max - min + 1);
    uint32_t x;
    uint32_t limit = UINT32_MAX - (UINT32_MAX % range);

    do {
        x = Randomrand();

    } while (x >= limit);

    return (x & (range - 1)) + min;
}

const RandomInstance Random = {
    .seed = Randomseed, .rand = Randomrand, .randr = Randomrandr

};
