//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "../../types/main.h"

typedef struct {
    bytes_t (*djb2)(bytes_t src);
    bytes_t (*scorpion2048x)(bytes_t src);
    bytes_t (*scorpion1024x)(bytes_t src);
    bytes_t (*scorpion512x)(bytes_t src);
    bytes_t (*scorpion256x)(bytes_t src);
    bytes_t (*scorpion128x)(bytes_t src);
    
} HashInstance;

extern HashInstance Hash;