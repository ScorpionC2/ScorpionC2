//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "src/core/infra/sched/tasks/main.h"
#include "src/core/infra/sched/context/main.h"
#include "src/core/infra/sched/processor/main.h"
#include "src/core/infra/sched/tasks/stack/main.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Initialize task.
 *
 * @param func The function that will run in the task.
 * @param arg Pointer to the @p func argument.
 *
 * @return Pointer to the new task.
 * @retval NULL The given @p func parameter is NULL.
 * @retval NULL Allocation error.
 * @retval NULL Can't init stack.
 *
 * @note The returned SchedTask_t instance is owned by caller, that must release it.
 * @note To correctly release the SchedTask_t instance you must follow this checklist:
 *       1. Exit the task: You must run the _exitTask() funciton to ensure the DEAD state
 *          and a valid cleaned stack;
 *       2. Free the SchedTask_t instance with free().
 *
 * @attention The argument is inserted in the stack and must be popped using:
 *                  pop rdi
 *            before the function call.
 *
 * @pre @p func being a valid function pointer.
 *
 * @post @p func unchanged.
 * @post Valid task returned.
 */
struct SchedTask_t *_initTask(void (*func)(void *), void *arg) {
    if (func == NULL)
        return NULL;

    struct SchedTask_t *out = malloc(sizeof(struct SchedTask_t));
    if (out == NULL)
        return NULL;

    void *stack = _initStack();
    if (stack == NULL) {
        free(out);
        return NULL;
    }

    out->stack = *(struct SchedTask_Stack_t *)(stack);
    out->func = func;
    out->arg = arg;
    out->state = RUNNABLE;

    // Adds the func to the stack so the ret op can handle it
    // Adds the arg and the _exitTask too
    uint64_t *stack_top = (uint64_t *)out->stack.top;
    *(--stack_top) = (uint64_t)_exitTask;
    *(--stack_top) = (uint64_t)out->func;
    *(--stack_top) = (uint64_t)out->arg;
    *(--stack_top) = (uint64_t)_task_entry_trampoline;

    // Every register turns to 0x00000000 and set rsp
    memset(&out->ctx, 0x00, sizeof(struct SchedContext_t));
    out->ctx.rsp = (uint64_t)stack_top;

    return out;
}

/**
 * @brief Exit task.
 *
 * Gracefully exit the task by turning it state to DEAD,
 * cleaning it's stack, swaping the context to the processor
 * thread and setting the __builtin_unreachable flag.
 */
void _exitTask() {
    currentProcessor->current->state = DEAD;
    _cleanStack(&currentProcessor->current->stack);
    _context_swap_linux_amd64(&currentProcessor->current->ctx,
                              &currentProcessor->context);

    __builtin_unreachable();
}
