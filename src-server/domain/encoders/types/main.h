//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "src-server/infra/hash/main.h"
#include "src-server/shared/types/main.h"
#include <stdint.h>

typedef struct {
    short shortNum;
    int num;
    int *numArr;
    uint32_t uint;
    uint32_t *uintArr;
    long longInt;
    long *longIntArr;
    uint64_t ulong;
    uint64_t *ulongArr;
    char character;
    char *string;
    char **stringArr;
    uchar_t uchar;
    uchar_t *bytes;
    bool_t boolean;
    bool_t *booleanArr;
    double floatingPoint;
    double *floatingPointArr;
    scorpionSettings *hashScorpionSettings;

} EncoderSettings;

typedef struct {
    bytes_t (*encode)(bytes_t src);
    bytes_t (*decode)(bytes_t src);

    /*
     * Settings must have default values for used properties
     * 
     * Settings must be well-documented
     */
    EncoderSettings *settings;

} EncoderInstance;
