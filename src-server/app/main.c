//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "cli/logs/main.h"
#include "cli/colors/main.h"
#include "../shared/types/main.h"
#include "../shared/utils/random/main.h"
#include "../shared/utils/hash/main.h"
#include "../domain/encoders/services/main.h"

#include <inttypes.h>
#include <time.h>
#include <stdlib.h>

int main() {
    Random.seed(time(NULL));
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
    
    Logger.infof(FG_WHITE_ITALIC "Random number: " RESET BG_BLUE FG_WHITE_ITALIC "%"PRIu32 RESET "\n", Random.rand());
    Logger.infof(FG_WHITE_ITALIC "Random number (0-255): " RESET BG_BLUE FG_WHITE_ITALIC "%"PRIu32 RESET "\n", Random.randr(0, 255));
    
    bytes_t xored = Xor->encode(nameBytes);
    Logger.bytesf(FG_WHITE_ITALIC "Xored string \"howo\": " RESET BG_GREEN FG_WHITE_ITALIC "%bh" RESET "\n", &xored);
    
    bytes_t unxored = Xor->decode(xored);
    Logger.bytesf(FG_WHITE_ITALIC "Unxored string \"howo\": " RESET BG_GREEN FG_WHITE_ITALIC "%bh" RESET "\n", &unxored);
    
    free(xored.b);
    free(unxored.b);
    
    Xor->settings->num = 16;
    bytes_t xored16 = Xor->encode(nameBytes);
    Logger.bytesf(FG_WHITE_ITALIC "Xored string with nonce size 16 \"howo\": " RESET BG_GREEN FG_WHITE_ITALIC "%bh" RESET "\n", &xored16);
    
    bytes_t unxored16 = Xor->decode(xored16);
    Logger.bytesf(FG_WHITE_ITALIC "Unxored string with nonce size 16 \"howo\": " RESET BG_GREEN FG_WHITE_ITALIC "%bh" RESET "\n", &unxored16);
    
    free(xored16.b);
    free(unxored16.b);

    bytes_t djb2 = Hash.djb2(nameBytes);
    Logger.bytesf(FG_WHITE_ITALIC "Hash for string \"howo\" (djb2): " RESET BG_PINK FG_WHITE_ITALIC "%bh" RESET "\n", &djb2);
    
    return 0;
    
}