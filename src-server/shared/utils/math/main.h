//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <stdint.h>

static inline uint32_t rotl(const uint32_t x, const int r) {
    return (x << (r & 31)) | (x >> (32 - (r & 31)));

}

static inline uint32_t rotr(const uint32_t x, const int r) {
    return (x >> (r & 31)) | (x << (32 - (r & 31)));

}

static inline uint8_t rotl8(const uint8_t x, const uint8_t n) {
    return (x << (n & 7)) | (x >> (8 - (n & 7)));

}

static inline uint8_t rotr8(const uint8_t x, const uint8_t n) {
    return (x >> (n & 7)) | (x << (8 - (n & 7)));

}

static inline uint16_t rotl16(const uint16_t x, const uint8_t n) {
    return (x << (n & 15)) | (x >> (16 - (n & 15)));

}

static inline uint16_t rotr16(const uint16_t x, const uint8_t n) {
    return (x >> (n & 15)) | (x << (16 - (n & 15)));

}