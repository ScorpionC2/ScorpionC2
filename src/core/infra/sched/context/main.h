//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <stdint.h>

struct SchedContext_t {
    uint64_t rsp;
    uint64_t rbx;
    uint64_t rbp;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
};

extern void _context_get_linux_amd64(struct SchedContext_t *ctx);
extern void _context_swap_linux_amd64(struct SchedContext_t *old,
                                      struct SchedContext_t *new);
extern void _context_set_linux_amd64(struct SchedContext_t *ctx);
