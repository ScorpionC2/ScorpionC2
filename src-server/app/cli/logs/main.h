//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

typedef struct {
    void (*warn)(const char*);
    void (*info)(const char*);
    void (*error)(const char*);
    void (*debug)(const char*);

} LoggerInstance;

extern const LoggerInstance Logger;