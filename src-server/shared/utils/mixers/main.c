//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "src-server/shared/utils/mixers/main.h"
#include "src-server/shared/types/main.h"
#include "src-server/shared/utils/math/main.h"
#include <stdint.h>

void arxmix32(uint32_t *state, const int *_r, const int *_i, int wordLen) {
    int r = *_r;
    int i = *_i;

    state[r & (wordLen - 1)] += state[(r + 82) & (wordLen - 1)];
    state[r & (wordLen - 1)] ^= state[(i + 19) & (wordLen - 1)];
    state[r & (wordLen - 1)] *= rotr((state[r & (wordLen - 1)]),
                                     (state[(i + 83) & (wordLen - 1)] & 31));
    state[r & (wordLen - 1)] ^= state[(r + 52) & (wordLen - 1)] ^
                                rotr((state[(i + 24) & (wordLen - 1)]),
                                     (state[(r + 31) & (wordLen - 1)] & 31));

    state[r & (wordLen - 1)] += state[(i + r + 7) & (wordLen - 1)];
    state[r & (wordLen - 1)] *= 0x9E3779B1;
    state[r & (wordLen - 1)] ^= state[r & (wordLen - 1)] >> 16;
}

void arraymix32(uint32_t *state, const int *index, int wordLen, const int *_i,
                int miniWordLen) {
    int i = *_i;
    uint32_t mask = wordLen - 1;

    state[*index & mask] ^= state[(*index + i + 1) & mask];
    state[*index & mask] *=
        rotl(state[*index & mask], state[(*index + i + 1) & mask] & 31);
    state[*index & mask] ^= rotl(state[*index & mask], 16);

    const struct minimix_src indexes = {
        .src = {state[(*index + i + 2) & (wordLen - 1)],
                state[(*index + i + 10) & (wordLen - 1)],
                state[(*index + i + 21) & (wordLen - 1)]}};

    minimix32(indexes, &state[*index]);

    state[*index & mask] ^= 0x838383FF;
    state[*index & mask] += state[(*index + 44) & mask];
    state[*index & mask] ^= state[i & mask] * 0x9E37D9BF;
    state[*index & mask] += 1;
}

void multiarxmix32(uint32_t *state, int wordLen, const int *_i, bytes_t src) {
    int i = *_i;

    for (int r = 0; r < 4; r++) {
        uint32_t currentByte = src.b[i % src.len] ^ 0xAB808DF1;
        uint32_t mix = currentByte ^ rotl(src.b[(i + 1) % src.len], 0xBEEA5123);

        flavourmix32(&state[r & (wordLen - 1)], currentByte ^ mix);
        arxmix32(state, &r, &i, wordLen);
    }
}

void dependency32(int wordLen, uint32_t *state, const int *index) {
    for (int r = 0; r < 4; r++)
        arxmix32(state, &r, index, wordLen);

    // This sub-block will ensure that high bits influence low bits
    for (int ii = 0; ii < 4; ii++)
        state[ii] ^= state[(ii + 1) & (wordLen - 1)] >> 1;
}

void smallmix32(uint32_t *word) {
    uint32_t w = *word;

    for (int i = 0; i < 4; i++) {
        w *= 0x140329DD ^ (i | 1);
        w ^= 0x912125FF;
        w ^= w * rotr(w, 0x13371337);
    }

    *word = w;
};

void smallmix16(uint16_t *word) {
    uint16_t w = *word;

    for (int i = 0; i < 4; i++) {
        w *= 0x98430E0D ^ (i | 1);
        w ^= 0x1337;
        w ^= w * rotr16(w, 0xF00F);
    }

    *word = w;
}

void flavourmix32(uint32_t *word, uint32_t flavour) {
    uint32_t w = *word;

    for (int i = 0; i < 4; i++) {
        w ^= flavour;
        w *= 0x3008B987;
        w = rotr(w, 11);
        w ^= rotl(flavour, 7);
    }

    *word = w;
}

void flavourmix16(uint16_t *word, uint16_t flavour) {
    uint16_t w = *word;

    for (int i = 0; i < 4; i++) {
        w ^= flavour;
        w *= 0xCEE1;
        w = rotl16(w, 3);
        w ^= rotr16(flavour, 15);
    }

    *word = w;
};

inline uint16_t summarize16(uint32_t word) {
    smallmix32(&word);

    uint16_t a = word & 0xFFFF;
    uint16_t b = (word >> 16) & 0xFFFF;

    uint16_t out = 0;
    for (int i = 7; i >= 1; i -= 2) {
        out <<= 1;
        out |= (a >> i) & 1;

        out <<= 1;
        out |= (b >> (15 - i)) & 1;
    }

    return out;
}

void mixtwo32(uint32_t *state, bytes_t *src, int wordLen) {
    uint32_t mask = wordLen - 1;
    size_t srcLen = src->len;
    for (int i = 0; i < 8; i++) {
        const struct minimix_src stateMinimixSrc = {
            .src = {state[i & mask], state[(i + 1) & mask],
                    state[(i + 50) & mask]}};

        const struct minimix_src srcMinimixSrc = {
            .src = {src->b[wrap_idx(i, srcLen)],
                    src->b[wrap_idx(i + 2, srcLen)],
                    src->b[wrap_idx(i + 27, srcLen)]}};

        minimix32(srcMinimixSrc, &state[(i + 1) & mask]);

        uint32_t curByte = src->b[wrap_idx(i, srcLen)] |
                           (src->b[wrap_idx(i + 1, srcLen)] << 8) |
                           (src->b[wrap_idx(i + 2, srcLen)] << 16) |
                           (src->b[wrap_idx(i + 3, srcLen)] << 24);

        minimix32(stateMinimixSrc, &curByte);
        uint16_t srcA = curByte & 0xFFFF;
        uint16_t srcB = (curByte >> 16) & 0xFFFF;

        minimix16(stateMinimixSrc, &srcA);
        minimix16(stateMinimixSrc, &srcB);

        state[(i + 3) & mask] = srcA | ((uint32_t)srcB << 16);
    }
}
