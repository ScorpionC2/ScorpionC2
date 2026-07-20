//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "src/core/shared/types/main.h"
#include "src/core/shared/utils/math/main.h"
#include <stdint.h>

static inline uint32_t fmix32(uint32_t h) {
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

static inline uint32_t seed_helper(uint32_t x, uint32_t seed, uint32_t shift) {
    uint32_t y = x;
    uint32_t nseed = (seed << (shift & 0x1F)) ^ ((x >> 1) * 0x11F11E1D);

    x ^= (x >> (shift & 0x1F)) * (seed | 0x849012FD);
    y ^= (x >> (shift & 0x1F)) ^ ((seed << 1) ^ (x * seed));
    x ^= seed;

    nseed ^= x * (seed << (shift & 0x1F));
    nseed ^= seed;
    nseed = fmix32(nseed);

    x ^= y;
    y *= seed ^ (x << 29);
    x *= 0xB83910FA;
    x ^= nseed;
    x ^= y;

    return x & 0xFFFFFFFF;
}

static inline void arxmix8(uint8_t *word, uint32_t srcWord) {
    uint8_t msw0 = srcWord & 0xFF;
    uint8_t msw1 = (srcWord >> 8) & 0xFF;
    uint8_t msw2 = (srcWord >> 16) & 0xFF;
    uint8_t msw3 = (srcWord >> 24) & 0xFF;

    uint8_t w = *word;
    uint8_t old = w;

    w ^= rotl8(msw0, w);
    w += msw2 ^ rotl8(w, msw0);
    w ^= msw3 * rotr8(msw1, w + 1);

    w ^= old;
    *word = w;
}

struct minimix_src {
    uint32_t src[3];
};

static inline void minimix32(const struct minimix_src src, uint32_t *pword) {
    uint32_t word = *pword;
    word *= 0x39D652DB;

    uint8_t mw0 = word & 0xFF;
    uint8_t mw1 = (word >> 8) & 0xFF;
    uint8_t mw2 = (word >> 16) & 0xFF;
    uint8_t mw3 = (word >> 24) & 0xFF;

    arxmix8(&mw0, src.src[1]);
    arxmix8(&mw1, src.src[0]);
    arxmix8(&mw2, src.src[2]);
    arxmix8(&mw3, word);

    *pword = mw0 | (mw1 << 8) | (mw2 << 16) | (mw3 << 24);
}

static inline void minimix16(const struct minimix_src src, uint16_t *pword) {
    uint16_t word = *pword;

    uint8_t mw0 = word & 0xFF;
    uint8_t mw1 = (word >> 8) & 0xFF;

    arxmix8(&mw0, src.src[1]);
    arxmix8(&mw1, src.src[0]);

    *pword = mw0 | (uint16_t)(mw1 << 8);
}

void arxmix32(uint32_t *state, const int *_r, const int *_i, int wordLen);
void arraymix32(uint32_t *state, const int *index, int wordLen, const int *_i);
void multiarxmix32(uint32_t *state, int wordLen, const int *_i, bytes_t src);
void dependency32(int wordLen, uint32_t *state, const int *index);
void smallmix32(uint32_t *word);
void smallmix16(uint16_t *word);
void flavourmix32(uint32_t *word, uint32_t flavour);
void flavourmix16(uint16_t *word, uint16_t flavour);
uint16_t summarize16(uint32_t word);
