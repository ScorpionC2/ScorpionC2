//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "src-server/app/cli/colors/main.h"
#include "src-server/app/cli/logs/main.h"
#include "src-server/infra/hash/main.h"
#include "src-server/shared/types/main.h"
#include "tests/engine/main.h"
#include "tests/engine/unit/main.h"

#include <stdlib.h>
#include <string.h>

bool_t djb2_base(void) {
    uchar_t rawSource[5] = {0x0a, 0x41, 0x61, 0xff, 0x00};
    bytes_t source = {.b = (uchar_t *)rawSource, .len = sizeof(rawSource)};

    bytes_t hashedSource = Hash.djb2(source);
    bytes_t hashedSource_1 = Hash.djb2(source);

    bool_t out = TEST_EQUAL_BYTES(hashedSource, hashedSource_1);

    free(hashedSource.b);
    free(hashedSource_1.b);

    return out;
}

bool_t djb2_similarPayloadsColision(void) {
    uchar_t rawSource[1024 * 256]; // 256KB stack
    memset(rawSource, 0x00, sizeof(rawSource));

    uchar_t rawModSource[sizeof(rawSource)];
    memcpy(rawModSource, rawSource, sizeof(rawSource));
    rawModSource[sizeof(rawSource) - 1] = 0x01;

    bytes_t source = {.b = (uchar_t *)rawSource, .len = sizeof(rawSource)};
    bytes_t modSource = {.b = (uchar_t *)rawModSource,
                         .len = sizeof(rawSource)};

    bytes_t hashedSouce = Hash.djb2(source);
    bytes_t hashedModSource = Hash.djb2(modSource);

    bool_t out = TEST_UNEQUAL_BYTES(hashedSouce, hashedModSource);

    free(hashedSouce.b);
    free(hashedModSource.b);

    return out;
}

// For contributors: TODO: Write a hash test suite

TESTS_UNIT_REGISTER(djb2_base, "DJB2 Hash");
TESTS_UNIT_REGISTER(djb2_similarPayloadsColision,
                    "DJB2 Similar Payloads Collision");
