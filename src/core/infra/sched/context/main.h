//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <stdint.h>

/**
 * @brief Scheduer Context Structure.
 *
 * This struct stores the values of the callee-saved registers
 * and the stack pointer.
 *
 * @note Don't initialize manually, use the _context_* wrappers.
 */
struct SchedContext_t {
    uint64_t rsp;
    uint64_t rbx;
    uint64_t rbp;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
};

/**
 * @brief Context Swapping for linux amd64 architecture.
 *
 * @param old Pointer to the SchedContext_t instance that will receive the old
 *        context backup.
 * @param new Pointer to the SchedContext_t instance that will be swapped with the
 *        old context, being the new context.
 */
extern void _context_swap_linux_amd64(struct SchedContext_t *old,
                                      struct SchedContext_t *new);

/**
 * @brief Context getting for linux amd64 architecture.
 *
 * @param ctx The SchedContext_t instance that'll receive the current context.
 */
extern void _context_get_linux_amd64(struct SchedContext_t *ctx);

/**
 * @brief Context setting for linux amd64 architecture.
 *
 * @param ctx the SchedContext_t instance that'll be used to set current context.
 */
extern void _context_set_linux_amd64(struct SchedContext_t *ctx);
