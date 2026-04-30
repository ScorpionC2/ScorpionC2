//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "src-server/shared/types/main.h"

#define ECHO_MODE (1 << 0)
#define CANONICAL_MODE (1 << 1)

typedef struct {
    void (*turnMode)(uint_t mode);

} TerminalModeChangerInstance;

extern TerminalModeChangerInstance TerminalModeToggler;
