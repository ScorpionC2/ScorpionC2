//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//
// This file uses the formula presented by:
//  https://arxiv.org/abs/1402.6246
//  https://en.wikipedia.org/wiki/Xorshift
//  https://www.youtube.com/watch?v=XDsYPXRCXAs
//  https://www.ams.org/journals/mcom/1999-68-225/S0025-5718-99-00996-5/S0025-5718-99-00996-5.pdf
// This file contains upgraded versions of that formula
// 
// Last updates and fixes was based in:
//  https://romailler.ch/2020/07/28/crypto-modulo_bias_guide/?utm_source=reddit.com
//  https://stackoverflow.com/questions/10984974/why-do-people-say-there-is-modulo-bias-when-using-a-random-number-generator?utm_source=google.com
//  https://www.pcg-random.org/posts/xoroshiro-fails-truncated.html?utm_source=reddit.com
//  https://en.wikipedia.org/wiki/Random_number_generation
//  https://en.wikipedia.org/wiki/Randomness_extractor?utm_source=google.com
// 
// ps: removed the large "docs" because it's useless
// 

#include "main.h"
#include <unistd.h>
#include <stdint.h>
#include <time.h>

uint32_t seed_g = 1;

void Randomseed(int seed) {
    seed_g = ((uint32_t)seed ^ ((((uint32_t)getpid() * (seed ^ ((uint32_t)time(NULL) >> 5))) * (time(NULL) * 571205793)) >> 9));
    
}

uint32_t Randomrseed(int seed) {
    return ((((seed * (time(NULL) ^ (uint32_t)getpid())) ^ (uint32_t)(seed / (uint32_t)getpid()) + (time(NULL) - (uint32_t)getpid())) * 2677735761u)) % (((((seed * seed) ^ seed) - 25) + 569821785) >> (seed % 8));
    
}

uint32_t Randomrand() {
    uint32_t s = seed_g;
    if (seed_g == 1) {
        seed_g ^= (seed_g * 257219671) + 0x9E3779B9;
        s = Randomrseed(seed_g ^ (3290751 * seed_g) + (Randomrseed(seed_g * 49091339) >> 15));
    
    }
    
    s = ((s << 13) ^ s) - (s >> 21);
    
    uint32_t n = s;
    n = (n * (n * n * 15732 + 789221) + 771171059);
    n += s;
    n = ((n << 14) ^ n) - (n >> 20);
    
    uint32_t x = n;
    x = (x * (x * 15731) + 2821160599);
    x += n;
    x = ((x << 19) ^ x) - (x >> 8);
    x += 0x9E3779B9;
    
    uint32_t y = x;
    y = (y * (y * y * 15731 + 789221) + 771171059);
    y ^= y >> 16;
    
    uint32_t z = y;
    z = (uint32_t)((z * 2650005761u) ^ z);
    
    uint32_t a = z;
    a = (a * (a * 0x9E3679B8u + a + (uint32_t)4322699887));
    
    uint32_t b = a;
    b = b * b * 0x9E3779B9u + b + 0x7F4A7C15u;
    b ^= b >> 28;
    
    uint32_t out = b ^ (b >> 8) ^ (b >> 16) ^ (b >> 24);
    seed_g ^= out + 0x9E3779B9;
    
    return out;
    
};

uint32_t Randomrandr(int min, int max) {
    uint32_t range = (uint32_t)(max - min + 1);
    uint32_t x;
    uint32_t limit = UINT32_MAX - (UINT32_MAX % range);    
    do {
        x = Randomrand();
    
    } while (x >= limit);
    
    return (x % range) + min;
    
}

const RandomInstance Random = {
    .seed = Randomseed,
    .rand = Randomrand,
    .randr = Randomrandr
    
};