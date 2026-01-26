//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "../../../shared/types/main.h"

typedef struct {
    bytes_t (*encode)(bytes_t src);
    bytes_t (*decode)(bytes_t src);
    
    /*
     * Settings must have default values
     * 
     * Settings must be well-documented
     */ 
    struct settings;

} EncoderInstance;