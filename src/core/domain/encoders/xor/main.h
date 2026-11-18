//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "src/core/infra/hash/main.h"
#include "src/core/shared/types/main.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Xor encoder settings structure.
 */
typedef struct {
    /**
     * @brief Nonce payload size.
     */
    size_t nonceSize;

    /**
     * @brief Hash (nonce derivated) algorithm.
     */
    enum {
        SCORPIONX,
        DJB2,
    } hashAlg;

    /**
     * @brief ScorpionX algorithm settings.
     */
    scorpionSettings scorpionxSettings;
} XorSettings;

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
bytes_t _xorEncode(bytes_t src);

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
bytes_t _xorDecode(bytes_t src);

/**
 * @brief Global xor encoder settings.
 */
extern XorSettings _xorConf;
