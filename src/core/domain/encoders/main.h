//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "src/core/domain/encoders/xor/main.h"
#include "src/core/shared/types/main.h"

/**
 * @brief Global encoder container, delivering every encoder available.
 */
typedef struct {
    /**
     * @brief Xor encoder instance.
     */
    struct {

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
        bytes_t (*encode)(bytes_t src);

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
        bytes_t (*decode)(bytes_t src);

        /**
         * @brief Global xor encoder settings.
         */
        XorSettings *settings;
    } Xor;
} EncoderInstance;

/**
 * @brief Global EncoderInstance instance.
 */
extern EncoderInstance Encoder;
