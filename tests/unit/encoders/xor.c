//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "src-server/domain/encoders/services/main.h"
#include "src-server/shared/types/main.h"
#include "src-server/shared/utils/random/main.h"
#include "tests/engine/main.h"
#include "tests/engine/unit/main.h"

#include <stdlib.h>

bool_t xor_base(void) {
    uchar_t rawSource[5] = {0xff, 0xf3, 0x0a, 0x00, 0x61};
    bytes_t source = {.b = (uchar_t *)rawSource, .len = sizeof(rawSource)};

    bytes_t encodedSource = Xor->encode(source);
    bytes_t decodedSource = Xor->decode(encodedSource);

    bool_t out = TEST_EQUAL_BYTES(source, decodedSource);

    free(encodedSource.b);
    free(decodedSource.b);

    return out;
}

bool_t xor_changeNonceLen(void) {
    int oldNonceLen = Xor->settings->num;
    Xor->settings->num = Random.randr(100, 300);

    uchar_t rawSource[5] = {0xff, 0xf3, 0x0a, 0x00, 0x61};
    bytes_t source = {.b = (uchar_t *)rawSource, .len = sizeof(rawSource)};

    bytes_t encodedSource = Xor->encode(source);
    bytes_t decodedSource = Xor->decode(encodedSource);

    Xor->settings->num = oldNonceLen;

    bool_t out = TEST_EQUAL_BYTES(source, decodedSource);

    free(encodedSource.b);
    free(decodedSource.b);

    return out;
}

bool_t xor_changeHashAlgorithm(void) {
    char *oldHashAlgorithm = Xor->settings->string;
    Xor->settings->string = "djb2";

    uchar_t rawSource[5] = {0xff, 0xf3, 0x0a, 0x00, 0x61};
    bytes_t source = {.b = (uchar_t *)rawSource, .len = sizeof(rawSource)};

    bytes_t encodedSource = Xor->encode(source);
    bytes_t decodedSource = Xor->decode(encodedSource);

    Xor->settings->string = oldHashAlgorithm;

    bool_t out = TEST_EQUAL_BYTES(source, decodedSource);

    free(encodedSource.b);
    free(decodedSource.b);

    return out;
}

bool_t xor_ensureNonceRandomness(void) {
    uchar_t rawSource[5] = {0xff, 0xf3, 0x0a, 0x00, 0x61};
    bytes_t source = {.b = (uchar_t *)rawSource, .len = sizeof(rawSource)};

    bytes_t encodedSource = Xor->encode(source);
    bytes_t encodedSource_2 = Xor->encode(source);

    bool_t out = TEST_UNEQUAL_BYTES(source, encodedSource_2);

    free(encodedSource.b);
    free(encodedSource_2.b);

    return out;
}

bool_t xor_changeTrashSize(void) {
    short oldTrashSize = Xor->settings->shortNum;
    Xor->settings->shortNum = Random.randr(100, 500);

    uchar_t rawSource[5] = {0xff, 0xf3, 0x0a, 0x00, 0x61};
    bytes_t source = {.b = (uchar_t *)rawSource, .len = sizeof(rawSource)};

    bytes_t encodedSource = Xor->encode(source);
    bytes_t decodedSource = Xor->decode(encodedSource);

    Xor->settings->shortNum = oldTrashSize;

    bool_t out = TEST_EQUAL_BYTES(source, decodedSource);

    free(encodedSource.b);
    free(decodedSource.b);

    return out;
}

TESTS_UNIT_REGISTER(xor_base, "Xor Test");
TESTS_UNIT_REGISTER(xor_changeNonceLen, "Xor With Changed Nonce Size");
TESTS_UNIT_REGISTER(xor_changeHashAlgorithm, "Xor With Changed Hash Algorithm");
TESTS_UNIT_REGISTER(xor_ensureNonceRandomness, "Xor Ensure Nonce Randomness");
TESTS_UNIT_REGISTER(xor_changeTrashSize, "Xor With Changed Trash Size");
