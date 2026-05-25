//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <stdint.h>
#include "src-server/shared/types/main.h"

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

struct minimix_src {
    uint32_t src[3];
};

void minimix32(const struct minimix_src src, uint32_t *pword);
void minimix16(const struct minimix_src src, uint16_t *pword);
void arxmix32(uint32_t *state, const int *_r, const int *_i, int wordLen);
void arxmix8(uint8_t *word, uint32_t srcWord);
void arraymix32(uint32_t *state, const int *index, int wordLen, const int *_i, int miniWordLen);
void multiarxmix32(uint32_t *state, int wordLen, const int *_i, bytes_t src);
void dependency32(int wordLen, uint32_t *state, const int *index);
void smallmix32(uint32_t *word);
void smallmix16(uint16_t *word);
void flavourmix32(uint32_t *word, uint32_t flavour);
void flavourmix16(uint16_t *word, uint16_t flavour);
uint16_t summarize16(uint32_t word);
