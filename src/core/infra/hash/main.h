//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <stdint.h>

#include "src-server/shared/types/main.h"

typedef struct {
    enum {
        small = 128,
        medium = 256,
        big = 512,
        giant = 1024,
        godzilla = 2048,

    } hashSize;

    uint32_t seed;
    uint32_t shiftSeed;

} scorpionSettings;

typedef struct {
    bytes_t (*djb2)(bytes_t src);
    bytes_t (*ScorpionX)(bytes_t src);
    scorpionSettings *settings;

} HashInstance;

extern HashInstance Hash;
