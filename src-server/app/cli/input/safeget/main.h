//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "../../../../shared/types/main.h"
#include <stddef.h>
#include <stdio.h>

typedef struct {    
    /*
     * Return rules:
     *      .s = NULL: Can't read or can't allocate new memory for string
     */
    string_t (*snread)(FILE *file, size_t size);

} SafeGetInstance;

extern SafeGetInstance SafeGet;