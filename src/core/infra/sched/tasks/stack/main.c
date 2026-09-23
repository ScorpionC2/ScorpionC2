//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "src/core/infra/sched/tasks/main.h"
#include "main.h"
#include "src/core/infra/sched/tasks/stack/main.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

/**
 * @brief Allocates virtual PROT_NONE memory.
 *
 * @param size Indicates the allocated memory size.
 *
 * @return Pointer to allocated memory.
 * @retval NULL Allocation failed.
 *
 * @pre @p size higher than 0.
 * @post Same @p.
 *
 * @note Works better if @p is power of 2.
 * @note The caller owns the allocated memory and must release it using munmap().
 */
void *_vallocNone(size_t size) {
    void *base =
        mmap(NULL, size, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    if (base == NULL || base == MAP_FAILED)
        return NULL;

    return base;
}

/**
 * @brief Commits allocated virtual memory.
 *
 * Commiting a memory block means that the allocated
 * memory, formerly PROT_NONE, will receive RW permissions
 * using PROT_READ and PROT_WRITE.
 *
 * @param ptr Pointer to allocated virtual memory.
 * @param size Allocated block size.
 * @param commit Size to be commited.
 *
 * @return Pointer to @p ptr.
 * @retval NULL mprotect() failed.
 *
 * @pre @p ptr poiting to a valid allocated block.
 * @pre @p size being the block size.
 * @pre @p commit being smaller than @p size.
 *
 * @post @p ptr with more capability.
 *
 * @note Works better if the @p commit is power of two.
 */
void *_vmcommitRW(void *ptr, size_t size, size_t commit) {
    void *stack_top = (char *)ptr + size;
    if (mprotect((char *)stack_top - commit, commit, PROT_READ | PROT_WRITE) !=
        0)
        return NULL;

    return ptr;
}

/**
 * @brief Initiates a pseudo-stack for the task.
 *
 * @return Pointer to the stack.
 * @retval NULL Memory error.
 *
 * @note The returned SchedTask_Stack_t instance is owned by caller, who must release it if isn't more needed.
 */
struct SchedTask_Stack_t *_initStack() {
    struct SchedTask_Stack_t *out = malloc(sizeof(struct SchedTask_Stack_t));
    *out = (struct SchedTask_Stack_t){
        .cap = DEFAULT_STACK_CAP,
        .commited = 0,
    };

    void *buf = _vallocNone(out->cap);
    if (buf == NULL)
        return NULL;

    void *commit = _vmcommitRW(buf, out->cap, DEFAULT_STACK_EXPAND_SIZE);
    if (commit == NULL)
        return NULL;

    out->buf = buf;
    out->commited += DEFAULT_STACK_EXPAND_SIZE;
    out->size = out->commited;

    return out;
}

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
void *_handleTaskStack(struct SchedTask_Stack_t *stack, uint64_t rsp) {
    if (stack == NULL)
        return NULL;

    if (stack->buf == NULL)
        return NULL;

    // Stack size (f)
    if ((stack->size - rsp) >= (stack->cap - STACK_CAP_MARGIN)) {
        stack->commited += DEFAULT_STACK_EXPAND_SIZE;
        void *commit = _vmcommitRW(stack->buf, stack->cap, stack->commited);
        if (commit == NULL)
            // TODO clean task exit
            return NULL;

        stack->size = stack->commited;
    }

    return stack;
}

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
bool_t _cleanStack(struct SchedTask_Stack_t *stack) {
    if (stack == NULL)
        return FALSE;

    // Virtual-memory cleaning
    if (stack->buf == NULL)
        return FALSE;

    if (munmap(stack->buf, stack->size) != 0)
        return FALSE;

    return TRUE;
}
