//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <stdint.h>

typedef struct {
    void (*seed)(int seed);
    uint32_t (*rand)();
    uint32_t (*randr)(int min, int max);
    
} RandomInstance;

extern const RandomInstance Random;