//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "main.h"
#include "safeget/main.h"
#include "../../../infra/fs/main.h"
#include "../logs/main.h"
#include <string.h>
#include <stdlib.h>

void readline(InputSettings conf, string_t *out) {
    string_t prompt = conf.prompt;
    string_t histPath = conf.histPath;
    
    printf("%s", prompt.s);
    fflush(stdout);
    string_t input = SafeGet.ucread(conf.histPath, prompt.len - 1);
    bytes_t inputRaw = {
        .len = input.len,
        .b = (uchar_t*)input.s
        
    };
    
    uchar_t _nl = '\n';
    bytes_t newLine = {
        .len = 1,
        .b = &_nl
        
    };
    
    if ((Files.appendFile(histPath, &inputRaw) != 0) || (Files.appendFile(histPath, &newLine))) {
        Logger.warnln("Can't write last user input to history file");
        
    }
    
    out->len = input.len;
    out->s = malloc(out->len + 1);
    memcpy(out->s, input.s, out->len);
    out->s[out->len] = '\0';
    
    free(input.s);
    return;
    
};

InputInstance Input = {
    .readline = readline
    
};