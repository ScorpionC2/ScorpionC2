# ScorpionC2 TODO List

## Issues

1. Create context to `app/cli/input/safeget` - [Issue AZ3gDTv8cS4qjkleCTfG](https://sonarcloud.io/project/issues?impactSoftwareQualities=MAINTAINABILITY&issueStatuses=OPEN%2CCONFIRMED&id=ScorpionC2_ScorpionC2&open=AZ3gDTv8cS4qjkleCTfG)

## New features

1. Create SocorpionDo (go-like fast and lightweight coroutines) ON-GOING
2. Create ScorpionSk (socket-like Parallel and Concurrent API)
3. Create SDCS (Secure Dissemination of Cryptographic Secrets) using an Encoder over TLS distribute cryptographic keys for Custom Secure Protocols

## Testing

1. Write tests to hashing following the upp-test-framework but using the same 150k hashes to every test: https://gist.github.com/Yyax13/bb90d152951d7b9e6ce3e4cf346b3188

## Deep Descriptions

### Task 1 of Testing: 

The test must be size-agnostic, so the result metric must changes with the hash-size.
No-need of speed testing

## Current Implementing

### ScorpionDo - Task 1 of New Features

Diagram:

```
    Sched                                                                                                                                                  
      │                       ┌────► Routine 1 ────► _taskInit ─────► context_swap(old: processor, new: task) ───► _exitTask ─────► free(task stack & task)
      │                       │                                                                                                                            
      ├───────► Processor ────┼────► Routine 2 ────► _taskInit ─────► context_swap(old: processor, new: task) ───► _exitTask ─────► free(task stack & task)
      │                       │                                                                                                                            
      │                       └────► Routine 3 ────► _taskInit ─────► context_swap(old: processor, new: task) ───► _exitTask ─────► free(task stack & task)
      │                                                                                                                                                    
      │                       ┌────► Routine 1 ────► _taskInit ─────► context_swap(old: processor, new: task) ───► _exitTask ─────► free(task stack & task)
      │                       │                                                                                                                            
      │───────► Processor ────┼────► Routine 2 ────► _taskInit ─────► context_swap(old: processor, new: task) ───► _exitTask ─────► free(task stack & task)
      │                       │                                                                                                                            
      ▼                       └────► Routine 3 ────► _taskInit ─────► context_swap(old: processor, new: task) ───► _exitTask ─────► free(task stack & task)
```

1. Create Context Struct - DONE in src/core/infra/sched/context/main.h
2. Create Context Swap Functions - DONE in src/core/infra/sched/context/main.s
3. Create Task Struct - DONE in src/core/infra/sched/main.h
4. Create Task Queue Struct - DONE in src/core/infra/sched/main.h
4. Create _initTask with pseudo-stack allocation - DONE in src/core/infra/sched/main.c
5. Create _exitTask with context swapping back to the processor - DONE in src/core/infra/sched/main.c
6. Implement function pointer inserting into task stack - DONE in src/core/infra/sched/main.c
7. Implement dynamic stack growing - ON-GOING
8. Implement processor initialize
9. Implement yield
10. Implement processor blocked-threads yielding
11. Implement processor next-task algorithm based in round-robin
12. Implement scheduler initialize
13. Implement scheduler loop
