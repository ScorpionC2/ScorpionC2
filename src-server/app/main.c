//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "cli/logs/main.h"
#include "cli/colors/main.h"
#include "../shared/types/main.h"

int main() {
    char name[5] = "howo";
    bytes_t nameBytes = {
        .b = (uchar_t*)"howo",
        .len = 5
        
    };
    
    Logger.bytesf(FG_WHITE_ITALIC "Bytes: " RESET BG_RED FG_WHITE_ITALIC "%b" RESET FG_WHITE_ITALIC "\nBytes with 0x delimiter: " RESET BG_RED FG_WHITE_ITALIC "%bh" RESET FG_WHITE_ITALIC "\nAddress to string: " RESET BG_RED FG_WHITE_ITALIC "%p" RESET "\n", &nameBytes, &nameBytes, name);
    
    Logger.debugf("Debugging: %s\n", name);
    Logger.warnf("Warning: %s\n", name);
    Logger.infof("Information: %s\n", name);
    Logger.errorf("Error: %s\n", name);
    
    return 0;
    
}