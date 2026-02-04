//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
// Implementation is based in the following contents:
//  https://www.reddit.com/r/C_Programming/comments/1go5ee2/how_do_you_get_user_input_safely/
//  https://www.ibm.com/docs/pt-br/i/7.6.0?topic=functions-fgets-read-string
// 

#include "main.h"
#include <stdlib.h>
#include <string.h>

string_t snread(FILE *file, size_t size) {
    string_t out;
    char buf[size];
    
    if (fgets(buf, 2048, file) == NULL) {
        out.len = 0;
        out.s = NULL;
        return out;
        
    }
    
    buf[strcspn(buf, "\n")] = '\0';
    out.len = strlen(buf);
    out.s = malloc(out.len);
    if (out.s == NULL) {
        out.len = 0;
        return out;
        
    }
    
    memcpy(out.s, buf, out.len);
    return out;
    
};

SafeGetInstance SafeGet = {
    .snread = snread
    
};