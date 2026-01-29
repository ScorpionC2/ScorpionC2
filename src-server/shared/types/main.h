//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <stddef.h>

typedef unsigned char uchar_t;
typedef unsigned long ulong_t;
typedef struct {
    uchar_t     *b;
    size_t      len;
    
} bytes_t;

typedef enum bool_t {
    TRUE = 1,
    FALSE = 0
    
} bool_t;

typedef struct {
    char        *s;
    size_t      len;
    
} string_t;