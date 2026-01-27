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

#include <stdint.h>
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

bytes_t HashScorpion2048x(bytes_t src) {
    bytes_t out;
    out.len = 2048;
    out.b = malloc(out.len);
    
    for (int i = 0; i < out.len; i++) {
        uint32_t s = (uint32_t)src.b[i % src.len];
        s = ((s << 13) ^ s) - (s >> 21);
        
        uint32_t n = s;
        n = (n * (n * n * 28732 + 910375) + 758127501);
        n += s;
        n = ((n << 14) ^ n) - (n >> 20);
        
        uint32_t x = n;
        x = (x * (x * 67912) + 5809127097);
        x += n;
        x = ((x << 19) ^ x) - (x >> 8);
        x += 0x9E3789B9;
        
        uint32_t y = x;
        y = (y * (y * y * 68912 + 120957) + 609182504);
        y ^= y >> 26;
        
        uint32_t z = y;
        z = (uint32_t)((z * 2654005761u) ^ z);
        
        uint32_t a = z;
        a = (a * (a * 0x9E3679F8u + a + (uint32_t)7095126095));
        
        uint32_t b = a;
        b = b * b * 0x9E3479B9u + b + 0x7F3A7C15u;
        b ^= b >> 28;
        
        b ^= b >> 16;
        b *= 0x85EBCA6Bu;
        b ^= b >> 13;
        b *= 0xC2B2AE35u;
        b ^= b >> 16;
        
        uint32_t v = b;
        
        out.b[i + 0] = (uint8_t)(v);
        out.b[i + 1] = (uint8_t)(v >> 8);
        out.b[i + 2] = (uint8_t)(v >> 16);
        out.b[i + 3] = (uint8_t)(v >> 24);
        
    }
    
    bytes_t output;
    output.len = out.len;
    output.b = malloc(out.len);
    
    memcpy(output.b, out.b, out.len);
    
    free(out.b);
    return output;
    
};

bytes_t HashScorpion1024x(bytes_t src) {
    bytes_t out;
    out.len = 1024;
    out.b = malloc(out.len);
    
    for (int i = 0; i < out.len; i++) {
        uint32_t s = (uint32_t)src.b[i % src.len];
        s = ((s << 6) ^ s) - (s >> 21);
        
        uint32_t n = s;
        n = (n * (n * n * 28732 + 910365) + 758127501);
        n += s;
        n = ((n << 14) ^ n) - (n >> 20);
        
        uint32_t x = n;
        x = (x * (x * 67912) + 5809127097);
        x += n;
        x = ((x << 19) ^ x) - (x >> 8);
        x += 0x9E3789B9;
        
        uint32_t y = x;
        y = (y * (y * y * 68912 + 120957) + 609182504);
        y ^= y >> 16;
        
        uint32_t z = y;
        z = (uint32_t)((z * 2654005761u) ^ z);
        
        uint32_t a = z;
        a = (a * (a * 0x9E3679F8u + a + (uint32_t)7095136095));
        
        uint32_t b = a;
        b = b * b * 0x9E3479B9u + b + 0x7F3A7C15u;
        b ^= b >> 28;
        
        b ^= b >> 16;
        b *= 0x85EBCA6Bu;
        b ^= b >> 13;
        b *= 0xC2B2AE35u;
        b ^= b >> 16;
        
        uint32_t v = b;
        
        out.b[i + 0] = (uint8_t)(v);
        out.b[i + 1] = (uint8_t)(v >> 8);
        out.b[i + 2] = (uint8_t)(v >> 16);
        out.b[i + 3] = (uint8_t)(v >> 24);
        
    }
    
    bytes_t output;
    output.len = out.len;
    output.b = malloc(out.len);
    
    memcpy(output.b, out.b, out.len);
    
    free(out.b);
    return output;
    
};

bytes_t HashScorpion512x(bytes_t src) {
    bytes_t out;
    out.len = 512;
    out.b = malloc(out.len);
    
    for (int i = 0; i < out.len; i++) {
        uint32_t s = (uint32_t)src.b[i % src.len];
        s = ((s << 13) ^ s) - (s >> 21);
        
        uint32_t n = s;
        n = (n * (n * n * 28732 + 910075) + 758127501);
        n += s;
        n = ((n << 14) ^ n) - (n >> 20);
        
        uint32_t x = n;
        x = (x * (x * 67912) + 5877727097);
        x += n;
        x = ((x << 19) ^ x) - (x >> 8);
        x += 0x9E3789B9;
        
        uint32_t y = x;
        y = (y * (y * y * 68912 + 120957) + 609182504);
        y ^= y >> 19;
        
        uint32_t z = y;
        z = (uint32_t)((z * 2654005761u) ^ z);
        
        uint32_t a = z;
        a = (a * (a * 0x9E3679F8u + a + (uint32_t)7095126895));
        
        uint32_t b = a;
        b = b * b * 0x9E3479B9u + b + 0x7F3A7C15u;
        b ^= b >> 28;
        
        b ^= b >> 16;
        b *= 0x85EBCA6Bu;
        b ^= b >> 13;
        b *= 0xC2B2AE35u;
        b ^= b >> 16;
        
        uint32_t v = b;
        
        out.b[i + 0] = (uint8_t)(v);
        out.b[i + 1] = (uint8_t)(v >> 8);
        out.b[i + 2] = (uint8_t)(v >> 16);
        out.b[i + 3] = (uint8_t)(v >> 24);
        
    }
    
    bytes_t output;
    output.len = out.len;
    output.b = malloc(out.len);
    
    memcpy(output.b, out.b, out.len);
    
    free(out.b);
    return output;
    
};

bytes_t HashScorpion256x(bytes_t src) {
    bytes_t out;
    out.len = 256;
    out.b = malloc(out.len);
    
    for (int i = 0; i < out.len; i++) {
        uint32_t s = (uint32_t)src.b[i % src.len];
        s = ((s << 13) ^ s) - (s >> 21);
        
        uint32_t n = s;
        n = (n * (n * n * 28732 + 910375) + 758127501);
        n += s;
        n = ((n << 17) ^ n) - (n >> 20);
        
        uint32_t x = n;
        x = (x * (x * 67912) + 7508275091);
        x += n;
        x = ((x << 19) ^ x) - (x >> 8);
        x += 0x9E3789B9;
        
        uint32_t y = x;
        y = (y * (y * y * 68992 + 120957) + 609182504);
        y ^= y >> 16;
        
        uint32_t z = y;
        z = (uint32_t)((z * 2654005761u) ^ z);
        
        uint32_t a = z;
        a = (a * (a * 0x9E3679F8u + a + (uint32_t)7095126095));
        
        uint32_t b = a;
        b = b * b * 0x9E3479B9u + b + 0x7F3A7C15u;
        b ^= b >> 28;
        
        b ^= b >> 16;
        b *= 0x85EBCA6Bu;
        b ^= b >> 13;
        b *= 0xC2B2AE35u;
        b ^= b >> 16;
        
        uint32_t v = b;
        
        out.b[i + 0] = (uint8_t)(v);
        out.b[i + 1] = (uint8_t)(v >> 8);
        out.b[i + 2] = (uint8_t)(v >> 16);
        out.b[i + 3] = (uint8_t)(v >> 24);
        
    }
    
    bytes_t output;
    output.len = out.len;
    output.b = malloc(out.len);
    
    memcpy(output.b, out.b, out.len);
    
    free(out.b);
    return output;
    
};

bytes_t HashScorpion128x(bytes_t src) {
    bytes_t out;
    out.len = 128;
    out.b = malloc(out.len);
    
    for (int i = 0; i < out.len; i++) {
        uint32_t s = (uint32_t)src.b[i % src.len];
        s = ((s << 13) ^ s) - (s >> 21);
        
        uint32_t n = s;
        n = (n * (n * n * 65821 + 910375) + 881900501);
        n += s;
        n = ((n << 14) ^ n) - (n >> 10);
        
        uint32_t x = n;
        x = (x * (x * 67912) + 5809127097);
        x += n;
        x = ((x << 19) ^ x) - (x >> 8);
        x += 0x9E3789B9;
        
        uint32_t y = x;
        y = (y * (y * y * 68912 + 120957) + 609182504);
        y ^= y >> 16;
        
        uint32_t z = y;
        z = (uint32_t)((z * 2654005761u) ^ z);
        
        uint32_t a = z;
        a = (a * (a * 0x9E3679F8u + a + (uint32_t)7095126095));
        
        uint32_t b = a;
        b = b * b * 0x9E3479B9u + b + 0x7F3A7C15u;
        b ^= b >> 28;
        
        uint32_t v = b;
        
        out.b[i + 0] = (uint8_t)(v);
        out.b[i + 1] = (uint8_t)(v >> 8);
        out.b[i + 2] = (uint8_t)(v >> 16);
        out.b[i + 3] = (uint8_t)(v >> 24);
        
    }
    
    bytes_t output;
    output.len = out.len;
    output.b = malloc(out.len);
    
    memcpy(output.b, out.b, out.len);
    
    free(out.b);
    return output;
    
};

HashInstance Hash = {
    .djb2 = HashDjb2,
    .scorpion2048x = HashScorpion2048x,
    .scorpion1024x = HashScorpion1024x,
    .scorpion512x = HashScorpion512x,
    .scorpion256x = HashScorpion256x,
    .scorpion128x = HashScorpion128x
    
};