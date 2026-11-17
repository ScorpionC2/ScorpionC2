//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <stddef.h>
#include <stdint.h>

#define INLINEX inline __attribute__((always_inline))

/**
 * @brief DEBUG_PRINT_SPEC Macro, used for debugging logs.
 *
 * Output example using both _DPSPEC and _DPDATA: /src/core/app/main.c:1337 main() -> Something went wrong.
 *
 * @note Only fmt-driven logging will support this.
 * @note Must be used as string for concatenation, e.g.: printf(_DPSPEC "Something went wrong", _DPDATA);
 */
#define _DPSPEC "/%s:%d %s() -> "

/**
 * @brief DEBUG_PRINT_DATA Macro, used for debugging logs.
 *
 * Output example using both _DPSPEC and _DPDATA: /src/core/app/main.c:1337 main() -> Something went wrong.
 *
 * @note Only fmt-driven logging will support this.
 * @note Must be used as the first varg as _DPSPEC does so.
 */
#define _DPDATA __FILE__, __LINE__, __func__

typedef unsigned char uchar_t;
typedef unsigned long ulong_t;
typedef unsigned short ushort_t;
typedef unsigned int uint_t;

typedef struct {
    uchar_t *b;
    size_t len;
} bytes_t;

typedef enum bool_t {
    TRUE = 1,
    FALSE = 0,
} bool_t;

typedef struct {
    char *s;
    size_t len;
} string_t;

/**
 * @brief Breaks a 64 bits integer in two 32 bits integers.
 *
 * Usage: uint32_t u32[2] = BREAK_64_32(u64);
 */
#define BREAK_64_32(src)                                                       \
    {                                                                          \
        (uint32_t)src & 0xFFFFFFFF,                                            \
        ((uint32_t)src >> 32) & 0xFFFFFFFF,                                    \
    }

/*
 * @brief Breaks a 64 bits integer in four 16 bits integers.
 *
 * Usage: uint16_t u16[4] = BREAK_64_16(u64);
 */
#define BREAK_64_16(src)                                                       \
    {                                                                          \
        (uint16_t)src & 0xFFFF,                                                \
        ((uint16_t)src >> 16) & 0xFFFF,                                        \
        ((uint16_t)src >> 32) & 0xFFFF,                                        \
        ((uint16_t)src >> 48) & 0xFFFF,                                        \
    }

/**
 * @brief Breaks a 64 bits integer in eight bytes (8 bits integer).
 *
 * Usage: uint8_t u8[8] = BREAK_64_8(u64);
 */
#define BREAK_64_8(src)                                                        \
    {                                                                          \
        (uint8_t)src & 0xFF,         (src >> 8) & 0xFF,  (src >> 16) & 0xFF,   \
        ((uint8_t)src >> 24) & 0xFF, (src >> 32) & 0xFF, (src >> 40) & 0xFF,   \
        ((uint8_t)src >> 48) & 0xFF, (src >> 56) & 0xFF,                       \
    }

/**
 * @brief Breaks a 32 bits integer in two 16 bits integers.
 *
 * Usage: uint16_t u16[2] = BREAK_32_16(u32);
 */
#define BREAK_32_16(src)                                                       \
    {                                                                          \
        (uint16_t)src & 0xFFFF,                                                \
        ((uint16_t)src >> 16) & 0xFFFF,                                        \
    }

/**
 * @brief Breaks a 32 bits integer in four 8 bits integers.
 *
 * Usage: uint8_t u8[4] = BREAK_32_8(u32);
 */
#define BREAK_32_8(src)                                                        \
    {                                                                          \
        (uint8_t)src & 0xFF,                                                   \
        ((uint8_t)src >> 8) & 0xFF,                                            \
        ((uint8_t)src >> 16) & 0xFF,                                           \
        ((uint8_t)src >> 24) & 0xFF,                                           \
    }

/**
 * @brief Breaks a 16 bits integer in two 8 bits integers.
 *
 * Usage: uint8_t u8[2] = BREAK_16_8(u16);
 */
#define BREAK_16_8(src)                                                        \
    {                                                                          \
        (uint8_t)src & 0xFF,                                                   \
        ((uint8_t)src >> 8) & 0xFF,                                            \
    }
