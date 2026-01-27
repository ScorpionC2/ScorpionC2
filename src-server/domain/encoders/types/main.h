//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "../../../shared/types/main.h"
#include <stdint.h>

typedef struct {
    int num;
    int *numArr;
    int **ArrayOfNumArr;
    uint32_t uint;
    uint32_t *uintArr;
    uint32_t **ArrayOfUintArr;
    long longInt;
    long *longIntArr;
    long **ArrayOfLongIntArr;
    uint64_t ulong;
    uint64_t *ulongArr;
    uint64_t **ArrayOfUlongArr;
    char character;
    char *string;
    char **stringArr;
    uchar_t uchar;
    uchar_t *ustring;
    uchar_t **ustringArr;
    bool_t boolean;
    bool_t *booleanArr;
    bool_t **ArrayOfBooleanArr;
    double floatingPoint;
    double *floatingPointArr;
    double **ArrayOfFloatingPoint;

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
