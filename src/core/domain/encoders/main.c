//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "src/core/domain/encoders/main.h"
#include "src/core/domain/encoders/xor/main.h"

/**
 * @brief Global EncoderInstance instance.
 */
EncoderInstance Encoder = {
    .Xor =
        {
            .encode = _xorEncode,
            .decode = _xorDecode,
            .settings = &_xorConf,
        },
};
