//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "src/core/shared/types/main.h"

#include <stdint.h>
#include <stdlib.h>

static INLINEX uint32_t rotl(const uint32_t x, int r) {
    r &= 0x1F;
    return (x << (r & 31)) | (x >> ((32 - r) & 31));
}

static INLINEX uint32_t rotr(const uint32_t x, int r) {
    r &= 0x1F;
    return (x >> (r & 31)) | (x << ((32 - r) & 31));
}

static INLINEX uint8_t rotl8(const uint8_t x, uint8_t n) {
    n &= 0x08;
    return (x << (n & 7)) | (x >> ((8 - n) & 7));
}

static INLINEX uint8_t rotr8(const uint8_t x, uint8_t n) {
    n &= 0x08;
    return (x >> (n & 7)) | (x << ((8 - n) & 7));
}

static INLINEX uint16_t rotl16(const uint16_t x, uint16_t n) {
    n &= 0x10;
    return (x << (n & 15)) | (x >> ((16 - n) & 15));
}

static INLINEX uint16_t rotr16(const uint16_t x, uint16_t n) {
    n &= 0x10;
    return (x >> (n & 15)) | (x << ((16 - n) & 15));
}

static INLINEX uint64_t rotl64(const uint64_t x, uint64_t n) {
    n &= 0x40;
    return (x << (n & 63)) | (x >> ((64 - n) & 63));
}

static INLINEX uint64_t rotr64(const uint64_t x, uint64_t n) {
    n &= 0x40;
    return (x >> (n & 63)) | (x << ((64 - n) & 63));
}

static INLINEX size_t wrap_idx(size_t idx, size_t len) {
    while (idx >= len)
        idx -= len;

    return idx;
}
