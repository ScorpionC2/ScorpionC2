//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "src/core/shared/types/main.h"

#ifndef MAX_SCHED_PROCESSORS
#define MAX_SCHED_PROCESSORS 4
#endif

#ifndef FLAG_DEBUG
#define USE_DEBUG (bool_t)(FALSE)
#else
#define USE_DEBUG (bool_t)(TRUE)
#endif
