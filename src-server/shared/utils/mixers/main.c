//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "src-server/shared/utils/mixers/main.h"
#include "src-server/shared/utils/math/main.h"
#include "src-server/shared/types/main.h"
#include <stdint.h>

void arxmix8(uint8_t *word, uint32_t srcWord) {
    uint8_t msw0 = srcWord & 0xFF;
    uint8_t msw1 = (srcWord >> 8) & 0xFF;
    uint8_t msw2 = (srcWord >> 16) & 0xFF;
    uint8_t msw3 = (srcWord >> 24) & 0xFF;

    uint8_t old = *word;

    *word += rotl8(msw0, *word);
    *word ^= rotr8(msw1, *word);
    *word *= msw2 ^ rotl8(*word, msw0);
    *word ^= msw3 * rotr8(msw1, *word + 1);
    *word *= rotl8(*word, msw0);

    *word ^= rotr8(*word, old);
}

void minimix32(uint32_t src[3], uint32_t *pword) {
    uint32_t curByte = src[0];
    uint32_t word = *pword;

    uint8_t mw0 = word & 0xFF;
    uint8_t mw1 = (word >> 8) & 0xFF;
    uint8_t mw2 = (word >> 16) & 0xFF;
    uint8_t mw3 = (word >> 24) & 0xFF;

    mw0 ^= rotl8(src[1] << 8, 3) + mw2;
    mw1 ^= rotr8(curByte << 16, 5) + src[2];
    mw2 ^= mw0 * (curByte & 0xFF);
    mw3 ^= mw1 ^ 0xC3;

    arxmix8(&mw0, src[1]);
    arxmix8(&mw1, src[0]);
    arxmix8(&mw2, src[2]);
    arxmix8(&mw3, word);

    *pword = mw0 | (mw1 << 8) | (mw2 << 16) | (mw3 << 24);
}

void arxmix32(uint32_t *state, int *_r, int *_i, int wordLen) {
    int r = *_r;
    int i = *_i;

    state[r] += state[(r + 82) & (wordLen - 1)];
    state[r] ^= state[(i + 19) & (wordLen - 1)];
    state[r] = rotr(state[r], state[(i + 83) & (wordLen - 1)] & 31);
    state[r] ^= state[(r + 52) & (wordLen - 1)] ^ rotr(state[(i + 24) & (wordLen - 1)], state[(r + 31) & (wordLen - 1)]);

    state[r] += state[(i + r - 7) & (wordLen - 1)] & 0xF;
    state[r] *= 0x9E3779B1;
    state[r] ^= state[r] >> 16;
}

void arraymix32(uint32_t *state, int *index, int wordLen, int *_i, int miniWordLen) {
    int i = *_i;

    state[*index] ^= state[(*index + i + 1) & (wordLen - 1)];
    state[*index] = rotl(state[*index], state[(*index + i + 1) & (miniWordLen - 1)] & 31);
    state[*index] = rotl(state[*index], 16);

    minimix32((uint32_t*)&state[(*index + i + 2) & (wordLen - 1)], &state[*index]);

    state[*index] ^= 0x838383FF;
    state[*index] += state[(*index + 44) & (wordLen - 1)];
    state[*index] ^= state[i & (wordLen - 1)] * 0x9E37D9BF;
    state[*index] += 1;
}

void multiarxmix32(uint32_t *state, int wordLen, int *_i, bytes_t src) {
    int i = *_i;

    for (int r = 0; r < wordLen; r++) {
        uint32_t currentByte = src.b[i % src.len] ^ 0xAB808DF1;
        uint32_t mix = currentByte ^ rotl(currentByte, src.b[(i + 1) % src.len]);

        arxmix32(state, &r, &i, wordLen);
    }
}

void dependency32(int wordLen, int miniWordLen, uint32_t *state, int *index) {
    for (int r = 0; r < wordLen; r++)
        arxmix32(state, &r, index, wordLen);

    // This sub-block will ensure that high bits influence low bits
    for (int ii = 0; ii < wordLen; ii++)
        state[ii] ^= state[(ii + 1) & (wordLen - 1)] >> 1;
}

void smallmix32(uint32_t *word) {
    uint32_t w = *word;

    for (int i = 0; i < 32; i++) {
        w ^= 0x912125FF;
        w *= rotl(w, 0x0870518D);
        w ^= rotr(w, *word);

        w *= w ^ rotl(w, 0xDEADBEEF);
        w ^= w * rotr(w, 0x13371337);
        w *= w << (rotr(w, 0x0539) & 0x1F);

        w ^= *word;
    }

    *word = w;
};

void flavourmix32(uint32_t *word, uint32_t flavour) {
    uint32_t w = *word;

    for (int i = 0; i < 32; i++) {
        smallmix32(&w);
        smallmix32(&flavour);

        w ^= flavour;
        w *= rotl(flavour, w);
        w ^= rotr(w, flavour);

        flavour ^= *word;

        w *= w ^ rotl(flavour, 0xDEADBEEF);
        w ^= w * rotr(flavour, w);
        w *= w << (rotl(flavour, 0x13371337) & 0x1F);

        w ^= *word;
    }

    *word = w;
}
