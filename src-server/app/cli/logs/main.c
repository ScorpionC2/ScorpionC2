//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "main.h"
#include "../colors/main.h"
#include <stdio.h>

void warn(const char *self) {
    printf(FG_YELLOW_BOLD "[!]" RESET FG_YELLOW_ITALIC " %s\n" RESET, self);

}

void info(const char *self) {
    printf(FG_GREEN "[?]" FG_GREEN_ITALIC " %s\n" RESET, self);

}

void error(const char *self) {
    printf(BG_YELLOW FG_RED_BOLD "[*]" RESET BG_YELLOW FG_RED_ITALIC " %s\n" RESET, self);

}

void debug(const char *self) {
    printf(FG_GREY_DARK "[DEBUG]" FG_GREY_DARK_ITALIC " %s\n" RESET, self);
    
}

const LoggerInstance Logger = {
    .debug = debug,
    .warn = warn,
    .info = info,
    .error = error
    
};