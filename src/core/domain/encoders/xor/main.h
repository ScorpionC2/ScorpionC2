//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "src/core/infra/hash/main.h"
#include "src/core/shared/types/main.h"

#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t nonceSize;
    enum { SCORPIONX, DJB2 } hashAlg;
    scorpionSettings scorpionxSettings;
} XorSettings;

extern bytes_t(_xorEncode)(bytes_t src);
extern bytes_t(_xorDecode)(bytes_t src);
extern XorSettings _xorConf;
