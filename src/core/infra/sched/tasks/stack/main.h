//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "src/core/shared/types/main.h"
#include <stddef.h>
#include <stdint.h>

#define DEFAULT_STACK_INIT_SIZE (1 << 13)   // 8 KiB
#define DEFAULT_STACK_CAP (1 << 19)         // 512 KiB
#define STACK_CAP_MARGIN (1 << 10)          // 1 KiB
#define DEFAULT_STACK_EXPAND_SIZE (1 << 13) // 8 KiB

/**
 * @brief The task stack.
 *
 * @note Must be initialized by _initStack().
 */
struct SchedTask_Stack_t {
    /**
     * @brief Pointer to buffer.
     *
     * @note Not full usable, as it needs to be expanded by _handleTaskStack() if needed.
     * @note Caller owns it and must release the memory using munmap() if it is no longer used.
     */
    uchar_t *buf;

    /**
     * @brief Buffer capacity.
     *
     * This field exists because not the full buffer is usable, and must be commited.
     * The usable capacity is stored here.
     */
    size_t cap;

    /**
     * @brief Buffer size.
     *
     * The full size of the buffer, not full-usable.
     */
    size_t size;

    /**
     * @brief Commited part lenght of the buffer.
     */
    size_t commited;

    /**
     * @brief Stack top offset
     */
    uint64_t top;
};

/**
 * @brief Initiates a pseudo-stack for the task.
 *
 * @return Pointer to the stack.
 * @retval NULL Memory error.
 *
 * @note The returned SchedTask_Stack_t instance is owned by caller, who must release it if isn't more needed.
 */
struct SchedTask_Stack_t *_initStack();

/**
 * @brief Automatic expansion for task stack.
 *
 * @param stack Pointer to the stack struct.
 * @param rsp Task's RSP register.
 *
 * @return Pointer to @p stack.
 * @retval NULL Memory error or null @p stack.
 *
 * @note Even with a not-null @p stack, if the @p stack->buf is NULL, the function will return an error.
 */
void *_handleTaskStack(struct SchedTask_Stack_t *stack, uint64_t rsp);

/**
 * @brief Correctly clean and release the stack.
 *
 * @param stack A pointer to the SchedTask_Stack_t instance.
 *
 * @return Clean execution status, as a boolean.
 * @retval FALSE Clean failed or parameter nullity fallback.
 * @retval TRUE Successfuly cleaned the stack.
 *
 * @pre @p stack being a valid pointer to a valid SchedTask_Stack_t instance.
 * @post @p stack cleaned and it's memory released.
 */
bool_t _cleanStack(struct SchedTask_Stack_t **stack);
