//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <stdint.h>

static inline int coalesce_int(int v, int d) { return v ? v : d; }
static inline int coalesce_uint32(uint32_t v, uint32_t d) { return v ? v : d; }
