//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "src-server/shared/types/main.h"
#include <string.h>

/*
 * ALERT
 * If you code in JAVA, PYTHON, C++, JS or any shit like this, get out, 
 * you won't understand this code section.
 */

#define __TESTS_ENGINE_BYTES_COMP_HELPER(op)                                   \
    if (a.len != b.len)                                                        \
        return FALSE;                                                          \
    if (a.b == NULL || b.b == NULL)                                            \
        return FALSE;                                                          \
    return memcmp(a.b, b.b, a.len) op 0;

// X should use X(name, ret_value)
#define __TESTS_ENGINE_BYTES_COMP_FUNC_DEF_TABLE                               \
    X(equal, __TESTS_ENGINE_BYTES_COMP_HELPER(==))                             \
    X(unequal, return _TESTS_ENGINE_bytes_equal(a, b) == TRUE ? FALSE : TRUE;)

#define X(name, ret_value)                                                     \
    static inline bool_t _TESTS_ENGINE_bytes_##name(bytes_t a, bytes_t b) {    \
        ret_value                                                              \
    }

__TESTS_ENGINE_BYTES_COMP_FUNC_DEF_TABLE

#undef X

#define _TESTS_ENGINE_TERNARY ? TRUE : FALSE
#define _TESTS_ENGINE_MEMCMP(a, b, t) memcmp(&a, &b, sizeof(t))
#define _TESTS_ENGINE_SCALAR(a, b, op) ((a)op(b))

#define TEST_EQUAL(a, b) _TESTS_ENGINE_SCALAR(a, b, ==) _TESTS_ENGINE_TERNARY;
#define TEST_UNEQUAL(a, b) _TESTS_ENGINE_SCALAR(a, b, !=) _TESTS_ENGINE_TERNARY;
#define TEST_BIGGER(a, b) _TESTS_ENGINE_SCALAR(a, b, >) _TESTS_ENGINE_TERNARY;
#define TEST_LOWER(a, b) _TESTS_ENGINE_SCALAR(a, b, <) _TESTS_ENGINE_TERNARY;
#define TEST_BIGGER_EQUAL(a, b)                                                \
    _TESTS_ENGINE_SCALAR(a, b, >=) _TESTS_ENGINE_TERNARY;
#define TEST_LOWER_EQUAL(a, b)                                                 \
    _TESTS_ENGINE_SCALAR(a, b, <=) _TESTS_ENGINE_TERNARY;

#define TEST_EQUAL_BYTES(a, b) _TESTS_ENGINE_bytes_equal(a, b);
#define TEST_UNEQUAL_BYTES(a, b) _TESTS_ENGINE_bytes_unequal(a, b);

bool_t run(void);
int main(void);
