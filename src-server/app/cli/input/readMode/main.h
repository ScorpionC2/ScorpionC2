//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "../../../../shared/types/main.h"

typedef struct {
    void (*turnEchoMode)(bool_t _em);
    void (*turnCanonicalMode)(bool_t _cm);
    void (*turnEchoeMode)(bool_t _eem);
    
} TerminalModeTogglerInstance;

extern TerminalModeTogglerInstance TerminalModeToggler;