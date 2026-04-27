//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "src-server/shared/types/main.h"
#include <stddef.h>
#include <stdint.h>

struct UnitTest {
    bool_t (*func)();
    string_t name;
};

extern size_t unitTestsIndex;
extern size_t unitTestsSize;
extern struct UnitTest *unitTests;

void registerUnitTest(struct UnitTest *test);
void runUnitTests(uint32_t *ac);

#define TESTS_UNIT_REGISTER(f, n)                                              \
    static void __reg_unit_test_##f(void) __attribute__((constructor));        \
    static void __reg_unit_test_##f(void) {                                    \
        registerUnitTest(&(struct UnitTest){                                   \
            .name = (string_t){.s = (char *)n, .len = strlen(n)},              \
            .func = (bool_t (*)())f});                                         \
    }
