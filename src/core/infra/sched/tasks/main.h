//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "src/core/infra/sched/context/main.h"
#include "src/core/infra/sched/tasks/stack/main.h"

#include <stdint.h>

/**
 * @brief Sheduler task structure.
 * @note Don't initialize manually, must be initialized by _initTask().
 */
struct SchedTask_t {
    /**
     * @brief Function that will run.
     *
     * @note Can't be NULL by default (if created by the _initTask*()),
     *       as the default initialization won't return a valid SchedTask_t
     *       pointer if the given func is NULL.
     */
    void (*func)(void *);

    /**
     * @brief Pointer to the function argument.
     *
     * @note Type validation must be made function-side.
     * @note Nullity must be checked function-side.
     */
    void *arg;

    /**
     * @brief Task context.
     *
     * The task context is every callee-saved registers and the
     * stack pointer.
     *
     * @note Don't initialize manually, must be created by _initTask() pseudo-context handling.
     */
    struct SchedContext_t ctx;

    /**
     * @brief Task stack.
     * @note Don't initialize manually, must be created by _initTask(), that
     *       will call _initStack() to handle stack creating.
     */
    struct SchedTask_Stack_t *stack;

    /**
     * @brief Task state.
     *
     * The task state determinates what'll processors do:
     *  - If RUNNABLE, the processor will run it;
     *  - If RUNNING, the processor is already running it;
     *  - If DEAD, the processor will not catch it and the task will be correctly deleted.
     *  - If WAITING, the processor will wait the I/O notfier to run the task.
     *
     * @note Don't modify manually, can break tasks.
     */
    enum taskState { RUNNABLE, RUNNING, DEAD, WAITING } state;
};

/**
 * @brief Trampoline to the task entry.
 * Just pop the stack to rdi and return (ret) to the next function in the stack.
 * This is inserted at the start of stack so the task'll do:
 *
 *  1. Trampoline (pop rdi, ret);
 *  2. Main function (address in the stack);
 *  3. _exitTask.
 *
 * The task stack order is:
 *
 *  *(--stack_top) = _exitTask;
 *  *(--stack_top) = _func;
 *  *(--stack_top) = void *arg;
 *  *(--stack_top) = _task_entry_trampoline;
 *
 * As the stack grows backwards, this ensure the execution order.
 *
 */
extern void _task_entry_trampoline();

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
 * @pre @p func being a valid function pointer.
 *
 * @post @p func unchanged.
 * @post Valid task returned.
 */
struct SchedTask_t *_initTask(void (*func)(void *), void *arg);

/**
 * @brief Exit task.
 *
 * Gracefully exit the task by turning it state to DEAD,
 * cleaning it's stack, swaping the context to the processor
 * thread and setting the __builtin_unreachable flag.
 */
void _exitTask();
