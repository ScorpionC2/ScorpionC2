//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "main.h"
#include "src-server/infra/hash/main.h"
#include "src-server/shared/utils/random/main.h"

#include <stdlib.h>
#include <string.h>

scorpionSettings hashSettings = {
    .hashSize = godzilla, .seed = 0xF1FB230E, .shiftSeed = 18};

/*
 * int num: Nonce size
 * short shortNum: trash size
 * char *string: Hash algorithm
 */
EncoderSettings XorSettings = {
    // Nonce size
    .num = 128,

    // Trash size
    .shortNum = 64,

    // The Algorithm
    //
    // Our algorithm is defined by a 5 bytes string
    //
    //  djb2:   djb2
    //  scox:  scorpionx
    //
    .string = "scox",
    .hashScorpionSettings = &hashSettings

};

bytes_t xor(bytes_t src, bytes_t key) {
    // Creates copies of src and key
    uchar_t *srcCopy = malloc(src.len);
    memcpy(srcCopy, src.b, src.len);

    uchar_t *keyCopy = malloc(key.len);
    memcpy(keyCopy, key.b, key.len);

    // Initialize out and iterate over it
    uchar_t out[src.len];
    for (int i = 0; i < src.len; i++) {
        // Out[i] = src[i] xored to key[i + 1] (length fallback)
        out[i] = srcCopy[i] ^ keyCopy[(i + 1) % key.len];

        // Key[i + 2] (with fallback) = key[i]
        keyCopy[(i + 2) & (key.len - 1)] = keyCopy[i % key.len];

        // Key[i + 1] (fallback) xored to key[i + 3] (fallback)
        keyCopy[(i + 1) & (key.len - 1)] ^= keyCopy[(i + 3) & (key.len - 1)];
    }

    // Init output and copy
    bytes_t output = {.len = src.len, .b = malloc(src.len)};
    memcpy(output.b, out, src.len);

    // Free src and key copies
    free(srcCopy);
    free(keyCopy);

    return output;
}

void *getHashFunction() {
    // str == "djb2"
    if (strcmp(XorSettings.string, "djb2") == 0) {
        return Hash.djb2;
    }

    // Default is using scorpionx
    // Set settings
    Hash.settings = XorSettings.hashScorpionSettings;
    return Hash.ScorpionX;
}

bytes_t XorEncode(bytes_t src) {
    // Init nonce and fill with random bytes
    uchar_t nonceRaw[XorSettings.num];
    for (int i = 0; i < XorSettings.num; i++) {
        nonceRaw[i] = Random.randr(0, 255);
    }

    bytes_t nonce = {.b = malloc(XorSettings.num), .len = XorSettings.num};
    memcpy(nonce.b, nonceRaw, (size_t)XorSettings.num);

    // Get hash function and create a hash based in nonce
    bytes_t (*hashFunction)(bytes_t src) = getHashFunction();
    bytes_t hash = hashFunction(nonce);

    // Xor using src and hash
    bytes_t xorOut = xor(src, hash);
    bytes_t out = {.len = XorSettings.num + src.len + XorSettings.shortNum + 4};

    // Init trash and copy
    uchar_t trashRaw[XorSettings.shortNum];
    for (int i = 0; i < XorSettings.shortNum; i++) {
        trashRaw[i] = Random.randr(0, 255);
    }

    bytes_t trash = {.len = XorSettings.shortNum};
    trash.b = malloc(trash.len);
    memcpy(trash.b, trashRaw, trash.len);

    // Init and copy nonceKey
    uchar_t nonceKeyRaw[4];
    for (int i = 0; i < sizeof(nonceKeyRaw); i++) {
        nonceKeyRaw[i] = Random.randr(0, 255);
    }

    bytes_t nonceKey = {.len = sizeof(nonceKeyRaw)};
    nonceKey.b = malloc(nonceKey.len);
    memcpy(nonceKey.b, nonceKeyRaw, nonceKey.len);

    // Xor nonce
    bytes_t nonceXor = xor(nonce, nonceKey);

    // Initialize out.b and copy everything
    out.b = malloc(out.len);
    memcpy(out.b, nonceKey.b, nonceKey.len);
    memcpy(out.b + nonceKey.len, nonceXor.b, nonceXor.len);
    memcpy(out.b + nonceKey.len + nonce.len, xorOut.b, xorOut.len);
    memcpy(out.b + nonceKey.len + nonce.len + xorOut.len, trash.b, trash.len);

    free(hash.b);
    free(nonce.b);
    free(xorOut.b);
    free(trash.b);
    free(nonceKey.b);
    free(nonceXor.b);

    return out;
};

bytes_t XorDecode(bytes_t src) {
    // Start src copy and copy src to it
    bytes_t srcCopy = {.len = src.len, .b = malloc(src.len)};
    memcpy(srcCopy.b, src.b, srcCopy.len);

    // Nonce parsing
    bytes_t nonce;
    nonce.len = XorSettings.num;
    nonce.b = malloc(XorSettings.num);
    memcpy(nonce.b, srcCopy.b + 4, XorSettings.num);

    // Unxor nonce
    bytes_t nonceKey;
    nonceKey.len = 4;
    nonceKey.b = malloc(nonceKey.len);
    memcpy(nonceKey.b, srcCopy.b, nonceKey.len);

    bytes_t nonceUnxored = xor(nonce, nonceKey);

    // Init unxored src and copy the src without nonce and trash
    bytes_t srcWithoutXor;
    srcWithoutXor.len =
        srcCopy.len - XorSettings.num - XorSettings.shortNum - 4;
    srcWithoutXor.b = malloc(srcWithoutXor.len);
    memcpy(srcWithoutXor.b, srcCopy.b + XorSettings.num + 4, srcWithoutXor.len);

    // Get hash and hash the nonce
    bytes_t (*hashFunction)(bytes_t src) = getHashFunction();
    bytes_t hash = hashFunction(nonceUnxored);

    // Xor the src without nonce (i need to change this shit name btw); Init and copy the result to out
    bytes_t unxor = xor(srcWithoutXor, hash);
    bytes_t out = {.len = unxor.len, .b = malloc(unxor.len)};
    memcpy(out.b, unxor.b, out.len);

    // Free everything
    free(srcCopy.b);
    free(nonce.b);
    free(srcWithoutXor.b);
    free(hash.b);
    free(unxor.b);
    free(nonceKey.b);
    free(nonceUnxored.b);

    return out;
};

EncoderInstance XorInstance = {
    .encode = XorEncode, .decode = XorDecode, .settings = &XorSettings

};
