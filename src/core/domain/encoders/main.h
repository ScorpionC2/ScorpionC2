//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "src/core/domain/encoders/xor/main.h"
#include "src/core/shared/types/main.h"

typedef struct {
    struct {
        bytes_t (*encode)(bytes_t src);
        bytes_t (*decode)(bytes_t src);
        XorSettings *settings;
    } Xor;
} EncoderInstance;

extern EncoderInstance Encoder;
