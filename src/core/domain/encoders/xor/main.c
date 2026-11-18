//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "src/core/domain/encoders/xor/main.h"
#include "src/core/infra/hash/main.h"
#include "src/core/shared/types/main.h"
#include "src/core/shared/utils/random/main.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRASH_BYTES_LEN_SIZE 8
#define NONCE_KEY_SIZE 4

/**
 * @brief Global xor encoder settings.
 */
XorSettings _xorConf = {
    .scorpionxSettings =
        {
            .hashSize = medium,
            .seed = 0xF1FB230E,
            .shiftSeed = 2,
        },
    .nonceSize = 128,
    .hashAlg = SCORPIONX,
};

/**
 * @brief Mutate xor encoding.
 *
 * @param src Source bytes.
 * @param key Key bytes.
 *
 * @return The encoded data.
 * @retval {.len = 0, .b = NULL} Memory allocation error.
 *
 * This algorithm works by mixing the source with the key, mutating the key while mixing.
 *
 * @pre @p src.b being a valid bytes_t instance with non-zero data length.
 * @pre @p key.b being a valid bytes_t instance with non-zero data length.
 */
bytes_t xor(bytes_t src, bytes_t key) {
    uchar_t *keyCopy = malloc(key.len);
    if (keyCopy == NULL)
        return (bytes_t){.len = 0, .b = NULL};

    memcpy(keyCopy, key.b, key.len);

    // Initialize out and iterate over it
    uchar_t out[src.len];
    for (size_t i = 0; i < src.len; i++) {
        // Out[i] = src[i] xored to key[i + 1] (length fallback)
        out[i] = src.b[i] ^ key.b[(i + 1) % key.len];

        // Key[i + 2] (with fallback) = key[i]
        keyCopy[(i + 2) & (key.len - 1)] = keyCopy[i % key.len];

        // Key[i + 1] (fallback) xored to key[i + 3] (fallback)
        keyCopy[(i + 1) & (key.len - 1)] ^= keyCopy[(i + 3) & (key.len - 1)];
    }

    // Init output and copy
    bytes_t output = {.len = src.len, .b = malloc(src.len)};
    if (output.b == NULL)
        return (bytes_t){.len = 0, .b = NULL};

    memcpy(output.b, out, src.len);
    free(keyCopy);

    return output;
}

/**
 * @brief Get a hash function defined at encoder settings.
 *
 * @return Hash function pointer.
 * @note ScorpionX is the default algorithm.
 */
void *getHashFunction() {
    if (_xorConf.hashAlg == DJB2)
        return Hash.djb2;

    // Default alg is scorpionx
    Hash.settings = &_xorConf.scorpionxSettings;
    return Hash.ScorpionX;
}

/**
 * @brief Get the trash length.
 *
 * This function is responsible for determining the trashLen based in the source lenght.
 * It works by determining the min/max for each srcLen and returning a random number in this range.
 *
 * @param srcLen The source length.
 *
 * @return The trash length.
 * @retval 0 @p srcLen is 0.
 */
size_t getTrashLen(size_t srcLen) {
    if (srcLen < 1)
        return 0;

    size_t cap = 0;
    size_t min = 0;

    if (srcLen < 1024) {
        cap = 819;
        min = 410;
    } else if (srcLen > 1024 && srcLen < 4096) {
        cap = 2458;
        min = 1229;
    } else if (srcLen > 4096 && srcLen < 16384) {
        cap = 6554;
        min = 3277;
    } else if (srcLen > 16384) {
        cap = floor(srcLen * 0.2);
        min = floor(srcLen * 0.1);
    }

    size_t trashLen = Random.randr(min, cap);
    return trashLen;
}

/**
 * @brief Xor Encoder.
 *
 * The data is encoded using the xor() function, the key being a hash derived from random data (nonce).
 *
 * Output payload scheme:
 * [4 bytes nonceKey][8 bytes trashLen][xored nonce][xored data][length-variable trash]
 *
 * @param src The source bytes_t instance.
 *
 * @return Encoded data.
 * @retval {.len = 0, .b = NULL} Memory allocation error.
 *
 * @note The same source WILL GIVE DIFFERENT OUTPUTS.
 * @pre @p src being a valid bytes_t instance with non-zero data length.
 */
bytes_t _xorEncode(bytes_t src) {
    // Init nonce and fill with random bytes
    uchar_t nonceRaw[_xorConf.nonceSize];
    for (size_t i = 0; i < _xorConf.nonceSize; i++)
        nonceRaw[i] = (uchar_t)Random.randr(0, 255);

    size_t trashLen = getTrashLen(src.len);

    bytes_t nonce = {
        .len = _xorConf.nonceSize,
        .b = nonceRaw,
    };

    bytes_t (*hashFunction)(bytes_t source) = getHashFunction();
    bytes_t hash = hashFunction(nonce);

    bytes_t xorOut = xor(src, hash);

    // Init trash and copy
    uchar_t trashRaw[trashLen];
    for (size_t i = 0; i < trashLen; i++)
        trashRaw[i] = Random.randr(0, 255);

    bytes_t trash = {
        .len = trashLen,
        .b = trashRaw,
    };

    // Init and copy nonceKey
    uchar_t nonceKeyRaw[4];
    for (size_t i = 0; i < sizeof(nonceKeyRaw); i++)
        nonceKeyRaw[i] = Random.randr(0, 255);

    bytes_t nonceKey = {
        .len = sizeof(nonceKeyRaw),
        .b = nonceKeyRaw,
    };

    // Xor nonce with nonceKey
    bytes_t nonceXor = xor(nonce, nonceKey);

    // Init and copy trashLenBytes
    uchar_t trashLenBytesRaw[8] = BREAK_64_8(trashLen);
    bytes_t trashLenBytes = {
        .len = sizeof(trashLenBytesRaw),
        .b = trashLenBytesRaw,
    };

    // Initialize out and copy everything
    bytes_t out = {
        .len = nonceKey.len + trashLenBytes.len + _xorConf.nonceSize + src.len +
               trashLen,
    };

    out.b = malloc(out.len);
    if (out.b == NULL)
        return (bytes_t){.len = 0, .b = NULL};

    memcpy(out.b, nonceKey.b, nonceKey.len);
    memcpy(out.b + nonceKey.len, trashLenBytes.b, trashLenBytes.len);
    memcpy(out.b + nonceKey.len + trashLenBytes.len, nonceXor.b, nonceXor.len);

    memcpy(out.b + nonceKey.len + trashLenBytes.len + nonce.len, xorOut.b,
           xorOut.len);

    memcpy(out.b + nonceKey.len + trashLenBytes.len + nonce.len + xorOut.len,
           trash.b, trash.len);

    free(hash.b);
    free(xorOut.b);
    free(nonceXor.b);

    // Out format: [nonceKey][trashLenBytes][nonceXor][xorOut][trash]
    return out;
};

/**
 * @brief Xor Decoder.
 *
 * @param src Encoded data.
 *
 * @return Decoded data.
 * @retval {.len = 0, .b = NULL} Memory allocation error.
 *
 * @note A valid payload may fail decoding IF the encoder settings change.
 * @pre @p src being a valid bytes_t with non-zero data length.
 */
bytes_t _xorDecode(bytes_t src) {
    // Nonce parsing
    bytes_t nonce = {.len = _xorConf.nonceSize};
    nonce.b = malloc(_xorConf.nonceSize);
    if (nonce.b == NULL)
        return (bytes_t){.len = 0, .b = NULL};

    memcpy(nonce.b, src.b + NONCE_KEY_SIZE + TRASH_BYTES_LEN_SIZE,
           _xorConf.nonceSize);

    // Trash len parsing
    size_t trashLen = 0;
    for (int i = 0; i < TRASH_BYTES_LEN_SIZE; i++)
        trashLen |= (uint64_t)src.b[NONCE_KEY_SIZE + i] << (i * 8);

    // Unxor nonce
    bytes_t nonceKey;
    nonceKey.len = 4;
    nonceKey.b = malloc(nonceKey.len);
    memcpy(nonceKey.b, src.b, nonceKey.len);

    bytes_t nonceUnxored = xor(nonce, nonceKey);

    // Init unxored src and copy the src without nonce and trash
    bytes_t srcWithoutXor;
    srcWithoutXor.len = src.len - _xorConf.nonceSize - trashLen -
                        NONCE_KEY_SIZE - TRASH_BYTES_LEN_SIZE;

    srcWithoutXor.b = malloc(srcWithoutXor.len);
    memcpy(srcWithoutXor.b,
           src.b + _xorConf.nonceSize + NONCE_KEY_SIZE + TRASH_BYTES_LEN_SIZE,
           srcWithoutXor.len);

    // Get hash and hash the nonce
    bytes_t (*hashFunction)(bytes_t source) = getHashFunction();
    bytes_t hash = hashFunction(nonceUnxored);

    // Xor the src without nonce (i need to change this shit name btw); Init and copy the result to out
    bytes_t unxor = xor(srcWithoutXor, hash);
    bytes_t out = {.len = unxor.len, .b = malloc(unxor.len)};
    memcpy(out.b, unxor.b, out.len);

    // Free everything
    free(nonce.b);
    free(srcWithoutXor.b);
    free(hash.b);
    free(unxor.b);
    free(nonceKey.b);
    free(nonceUnxored.b);

    return out;
};
