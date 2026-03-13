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