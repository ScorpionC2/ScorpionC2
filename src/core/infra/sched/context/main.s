;
; Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
; Licensed under the MIT license. See LICENSE file in the project root for details.
;

BITS 64

%define CTX_RSP_OFFSET 0
%define CTX_RBX_OFFSET 8
%define CTX_RBP_OFFSET 16
%define CTX_R12_OFFSET 24
%define CTX_R13_OFFSET 32
%define CTX_R14_OFFSET 40
%define CTX_R15_OFFSET 48

global _context_swap_linux_amd64
global _context_get_linux_amd64
global _context_set_linux_amd64

; extern void _context_swap_linux_amd64(struct SchedContext_t *old, struct SchedContext_t *new);
_context_swap_linux_amd64:
  mov [rdi + CTX_RSP_OFFSET], rsp
  mov [rdi + CTX_RBX_OFFSET], rbx
  mov [rdi + CTX_RBP_OFFSET], rbp
  mov [rdi + CTX_R12_OFFSET], r12
  mov [rdi + CTX_R13_OFFSET], r13
  mov [rdi + CTX_R14_OFFSET], r14
  mov [rdi + CTX_R15_OFFSET], r15

  mov rsp, [rsi + CTX_RSP_OFFSET]
  mov rbx, [rsi + CTX_RBX_OFFSET]
  mov rbp, [rsi + CTX_RBP_OFFSET]
  mov r12, [rsi + CTX_R12_OFFSET]
  mov r13, [rsi + CTX_R13_OFFSET]
  mov r14, [rsi + CTX_R14_OFFSET]
  mov r15, [rsi + CTX_R15_OFFSET]

  ret

; extern void _context_get_linux_amd64(struct SchedContext_t *ctx);
_context_get_linux_amd64:
  mov [rdi + CTX_RSP_OFFSET], rsp
  mov [rdi + CTX_RBX_OFFSET], rbx
  mov [rdi + CTX_RBP_OFFSET], rbp
  mov [rdi + CTX_R12_OFFSET], r12
  mov [rdi + CTX_R13_OFFSET], r13
  mov [rdi + CTX_R14_OFFSET], r14
  mov [rdi + CTX_R15_OFFSET], r15

  ret

; extern void _context_set_linux_amd64(struct SchedContext_t *ctx);
_context_set_linux_amd64:
  mov rsp, [rdi + CTX_RSP_OFFSET]
  mov rbx, [rdi + CTX_RBX_OFFSET]
  mov rbp, [rdi + CTX_RBP_OFFSET]
  mov r12, [rdi + CTX_R12_OFFSET]
  mov r13, [rdi + CTX_R13_OFFSET]
  mov r14, [rdi + CTX_R14_OFFSET]
  mov r15, [rdi + CTX_R15_OFFSET]

  ret
