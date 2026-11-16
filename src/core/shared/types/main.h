//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <stddef.h>
#include <stdint.h>

#define INLINEX inline __attribute__((always_inline))

/**
 * @brief DEBUG_PRINT_SPEC and DEBUG_PRINT_DATA Macros, used for debugging logs.
 */
#define _DPSPEC "/%s:%d %s() -> "
#define _DPDATA __FILE__, __LINE__, __func__

typedef unsigned char uchar_t;
typedef unsigned long ulong_t;
typedef unsigned short ushort_t;
typedef unsigned int uint_t;
typedef struct {
    uchar_t *b;
    size_t len;
} bytes_t;

typedef enum bool_t { TRUE = 1, FALSE = 0 } bool_t;

typedef struct {
    char *s;
    size_t len;
} string_t;

// Usage: uint32_t u32[2] = BREAK_64_32(u64);
#define BREAK_64_32(src)                                                       \
    {                                                                          \
        (src) & 0xFFFFFFFF,                                                    \
        (src >> 32) & 0xFFFFFFFF,                                              \
    }

// Usage: uint16_t u16[4] = BREAK_64_16(u64);
#define BREAK_64_16(src)                                                       \
    {                                                                          \
        (src) & 0xFFFF,                                                        \
        (src >> 16) & 0xFFFF,                                                  \
        (src >> 32) & 0xFFFF,                                                  \
        (src >> 48) & 0xFFFF,                                                  \
    }

// Usage: uint8_t u8[8] = BREAK_64_8(u64);
#define BREAK_64_8(src)                                                        \
    {                                                                          \
        (src) & 0xFF,       (src >> 8) & 0xFF,  (src >> 16) & 0xFF,            \
        (src >> 24) & 0xFF, (src >> 32) & 0xFF, (src >> 40) & 0xFF,            \
        (src >> 48) & 0xFF, (src >> 56) & 0xFF,                                \
    }

// Usage: uint16_t u16[2] = BREAK_32_16(u32);
#define BREAK_32_16(src)                                                       \
    {                                                                          \
        (src) & 0xFFFF,                                                        \
        (src >> 16) & 0xFFFF,                                                  \
    }

// Usage: uint8_t u8[4] = BREAK_32_8(u32);
#define BREAK_32_8(src)                                                        \
    {                                                                          \
        (src) & 0xFF,                                                          \
        (src >> 8) & 0xFF,                                                     \
        (src >> 16) & 0xFF,                                                    \
        (src >> 24) & 0xFF,                                                    \
    }

// Usage: uint8_t u8[2] = BREAK_16_8(u16);
#define BREAK_16_8(src)                                                        \
    {                                                                          \
        (src) & 0xFF,                                                          \
        (src >> 8) & 0xFF,                                                     \
    }
