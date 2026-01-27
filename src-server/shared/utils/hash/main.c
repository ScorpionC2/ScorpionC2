//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//
// Some sources and logics was taken from
//  djb2:
//      https://theartincode.stanis.me/008-djb2/
//      https://hackmd.io/@sIQnCbQ0T56A3KLAiNrlhQ/r1GtXjBVP
//      source code for crackme in https://crackmes.one/crackme/6973ebc6d735cd51e7a1aa97
// 

#include "main.h"

#include <string.h>
#include <stdlib.h>

bytes_t HashDjb2(bytes_t src) {
    ulong_t hash = 5381;
    
    for (size_t i = 0; i < src.len; i++) {
        hash = ((hash << 5) + hash) + src.b[i];
    }
    
    bytes_t out = {
        .len = sizeof(hash),
        .b = malloc(sizeof(hash))
        
    };
    
    memcpy(out.b, &hash, sizeof(hash));
    return out;
    
};

HashInstance Hash = {
    .djb2 = HashDjb2
    
};