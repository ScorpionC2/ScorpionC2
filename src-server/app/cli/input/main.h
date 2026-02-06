//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "../../../shared/types/main.h"

typedef struct {
    string_t    prompt;
    string_t    histPath;
    int         histLimit;

} InputSettings;

typedef struct {
    void        (*readline)(InputSettings conf, string_t *out);

} InputInstance;

extern InputInstance Input;
