;
; Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
; Licensed under the MIT license. See LICENSE file in the project root for details.
;

BITS 64

global _task_entry_trampoline

;
; @brief Trampoline to the task entry.
; Just pop the stack to rdi and return (ret) to the next function in the stack.
; This is inserted at the start of stack so the task'll do:
;
;  1. Trampoline (pop rdi, ret);
;  2. Main function (address in the stack);
;  3. _exitTask.
;
; The task stack order is:
;
;  *(--stack_top) = _exitTask;
;  *(--stack_top) = _func;
;  *(--stack_top) = void *arg;
;  *(--stack_top) = _task_entry_trampoline;
;
; As the stack grows backwards, this ensure the execution order.
;
_task_entry_trampoline:
  pop rdi
  ret
