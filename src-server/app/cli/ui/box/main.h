//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "../../../../shared/types/main.h"

#define B_TL_CORNER     "╭"
#define B_TR_CORNER     "╮"
#define B_BL_CORNER     "╰"
#define B_BR_CORNER     "╯"
#define B_HORIZONTAL    "─"
#define B_VERTICAL      "│"

typedef struct {
    // Print a box sized as h * w
    // 
    // Uses color and color2 to do gradient-like box
    // 
    // Input colors must be smt like:
    //      C_WHITE_BASE that expands to "255;255;255"
    void (*draw)(int h, int w, char color1[15], char color2[15]);
    
} BoxxerInstance;

extern const BoxxerInstance Box;