//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "cli/logs/main.h"
#include "cli/colors/main.h"
#include "cli/ui/box/main.h"
#include "cli/input/main.h"
#include "../shared/types/main.h"
#include "../shared/utils/hash/main.h"
#include "../shared/utils/random/main.h"
#include "../domain/encoders/services/main.h"

#include <inttypes.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

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
    
    Xor->settings->num = 128;
    Xor->settings->string = "s512x";
    
    bytes_t xoredScorpion512x = Xor->encode(nameBytes);
    Logger.bytesf(FG_WHITE_ITALIC "Xored string with hash algorithm scorpion512x \"howo\": " RESET BG_GREEN FG_WHITE_ITALIC "%bh" RESET "\n", &xoredScorpion512x);
    
    bytes_t unxoredScorpion512x = Xor->decode(xoredScorpion512x);
    Logger.bytesf(FG_WHITE_ITALIC "Unored string with hash algorithm scorpion512x \"howo\": " RESET BG_GREEN FG_WHITE_ITALIC "%bh" RESET "\n", &unxoredScorpion512x);
    
    free(xoredScorpion512x.b);
    free(unxoredScorpion512x.b);

    bytes_t djb2 = Hash.djb2(nameBytes);
    Logger.bytesf(FG_WHITE_ITALIC "\n\nHash for string \"howo\" (djb2): " RESET BG_PURPLE FG_WHITE_ITALIC "%bh" RESET "\n", &djb2);
    
    bytes_t scorpion128x = Hash.scorpion128x(nameBytes);
    Logger.bytesf(FG_WHITE_ITALIC "\nHash for string \"howo\" (scorpion128x): " RESET BG_PURPLE FG_WHITE_ITALIC "%bh" RESET "\n", &scorpion128x);
    
    bytes_t scorpion256x = Hash.scorpion256x(nameBytes);
    Logger.bytesf(FG_WHITE_ITALIC "\nHash for string \"howo\" (scorpion256x): " RESET BG_PURPLE FG_WHITE_ITALIC "%bh" RESET "\n", &scorpion256x);
    
    bytes_t scorpion512x = Hash.scorpion512x(nameBytes);
    Logger.bytesf(FG_WHITE_ITALIC "\nHash for string \"howo\" (scorpion512x): " RESET BG_PURPLE FG_WHITE_ITALIC "%bh" RESET "\n", &scorpion512x);
    
    bytes_t scorpion1024x = Hash.scorpion1024x(nameBytes);
    Logger.bytesf(FG_WHITE_ITALIC "\nHash for string \"howo\" (scorpion1024x): " RESET BG_PURPLE FG_WHITE_ITALIC "%bh" RESET "\n", &scorpion1024x);
    
    bytes_t scorpion2048x = Hash.scorpion2048x(nameBytes);
    Logger.bytesf(FG_WHITE_ITALIC "\nHash for string \"howo\" (scorpion2048x): " RESET BG_PURPLE FG_WHITE_ITALIC "%bh" RESET "\n", &scorpion2048x);
    
    free(djb2.b);
    free(scorpion128x.b);
    free(scorpion256x.b);
    free(scorpion512x.b);
    free(scorpion1024x.b);
    free(scorpion2048x.b);
    
    Logger.infoln("Gradient Box:");
    Box.draw(10, 22, C_GREEN_LIGHT, C_GREEN_DARK);
    
    string_t hist = {
        .s = "historyTopzera",
        .len = strlen("historyTopzera")
        
    };
    InputSettings inputConf = {
        .histPath = hist,
        .prompt = {
            .s = "c2 -> ",
            .len = 7
            
        },
        .histLimit = 20000
        
    };
    
    string_t absoluteInput;
    Input.readline(inputConf, &absoluteInput);
    if (absoluteInput.s == NULL) {
        return 1;
        
    }
    
    Logger.infof(FG_WHITE_ITALIC "Input: " RESET FG_WHITE_ITALIC BG_ORANGE "%s" RESET "\n", absoluteInput.s);
    
    free(absoluteInput.s);
    
    return 0;
    
}