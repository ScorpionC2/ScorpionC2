//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "../../../../shared/types/main.h"
#include <stddef.h>
#include <stdio.h>

typedef struct {    
    // ucfgets - Ultra-incredible-handmade galactic empire engineer's Custom FGETS
    char* (*ucfgets)(string_t histPath, int promptSiz);
    
    /*
     * Return rules:
     *      .s = NULL: Can't read or can't allocate new memory for string
     */
    string_t (*ucread)(string_t histPath, int promptSize);

} SafeGetInstance;

extern SafeGetInstance SafeGet;