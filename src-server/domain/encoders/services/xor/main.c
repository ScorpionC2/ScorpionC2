//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "main.h"
#include "../../../../shared/utils/random/main.h"
#include "../../../../shared/utils/hash/main.h"

#include <stdlib.h>
#include <string.h>

/*
 * int num: Nonce size
 */
EncoderSettings XorSettings = {
    // Nonce size
    .num = 128
    
};

bytes_t xor(bytes_t src, bytes_t key) {
    uchar_t *srcCopy = malloc(src.len);
    memcpy(srcCopy, src.b, src.len);
    
    uchar_t *keyCopy = malloc(key.len);
    memcpy(keyCopy, key.b, key.len);
    
    uchar_t out[src.len];
    for (int i = 0; i < src.len; i++) {
        out[i] = srcCopy[i] ^ keyCopy[(i + 1) % key.len];
        keyCopy[(i + 2) % key.len] = keyCopy[i % key.len];
        
    }
    
    bytes_t output = {
        .len = src.len,
        .b = malloc(src.len)
        
    };
    
    memcpy(output.b, out, src.len);
    free(srcCopy);
    free(keyCopy);
    
    return output;
    
}

bytes_t XorEncode(bytes_t src) {
    uchar_t nonceRaw[XorSettings.num];
    for (int i = 0; i < XorSettings.num; i++) {
        nonceRaw[i] = Random.randr(0, 255);
        
    }
    
    bytes_t nonce = {
        .b = malloc(XorSettings.num),
        .len = XorSettings.num
        
    };
    
    memcpy(nonce.b, nonceRaw, (size_t)XorSettings.num);
    
    bytes_t hash = Hash.djb2(nonce);
    bytes_t xorOut = xor(src, hash);
    bytes_t out = {
        .len = XorSettings.num + src.len 
        
    };
    
    out.b = malloc(out.len);
    memcpy(out.b, nonce.b, nonce.len);
    memcpy(out.b + nonce.len, xorOut.b, xorOut.len);
    
    free(hash.b);
    free(nonce.b);
    free(xorOut.b);
    
    return out;
    
};

bytes_t XorDecode(bytes_t src) {
    bytes_t srcCopy = {
        .len = src.len,
        .b = malloc(src.len)
        
    };
    
    memcpy(srcCopy.b, src.b, srcCopy.len);
    
    bytes_t nonce;
    nonce.len = XorSettings.num;
    nonce .b = malloc(XorSettings.num);
    memcpy(nonce.b, srcCopy.b, XorSettings.num);
    
    bytes_t srcWithoutXor;
    srcWithoutXor.len = srcCopy.len - XorSettings.num;
    srcWithoutXor.b = malloc(srcWithoutXor.len);
    memcpy(srcWithoutXor.b, srcCopy.b + XorSettings.num, srcWithoutXor.len);

    bytes_t hash = Hash.djb2(nonce);
    bytes_t unxor = xor(srcWithoutXor, hash);
    
    bytes_t out = {
        .len = unxor.len,
        .b = malloc(unxor.len)
        
    };
    
    memcpy(out.b, unxor.b, out.len);
    
    free(srcCopy.b);
    free(nonce.b);
    free(srcWithoutXor.b);
    free(hash.b);
    free(unxor.b);
    
    return out;
    
};

EncoderInstance XorInstance = {
    .encode = XorEncode,
    .decode = XorDecode,
    .settings = &XorSettings

};