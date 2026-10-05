# ScorpionC2 TODO List

## Issues

## **Tasks and Features Backlog**

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
                  ─────────┬────────────────────────────────────────────────────────────────────────────────────►
                           ▼                                                                               ▲     
                       Processor      ┌────────────────────────────────┐                                   │     
                           │          │                               Yes                                  │     
                           │          ▼                                │                                   │     
                           └────► Main Loop ──────► Next Task ────► Running? ──No──► Graceful Shutdown ────┘     
                                      │                 ▲                                                        
                                      ▼                 │                                                        
    │ _exit  ◄──── Stack Init ◄── Init Task             │                                                        
    │ _func                           │                 │                                                        
    │ _arg                            ▼                 │                                                        
    │ _entry                     Swap 2 Task            │                                                        
    ▼                                 │                 │                                                        
                                      ▼                 │                                                        
                                  Swap Back ────────────┘                                                        
```

1. Create Context Struct - DONE
2. Create Context Swap Functions - DONE
3. Create Task Struct - DONE
4. Create Task Queue Struct - DONE
4. Create _initTask with pseudo-stack allocation - DONE
5. Create _exitTask with context swapping back to the processor - DONE
6. Implement function pointer inserting into task stack - DONE
7. Implement dynamic stack growing - DONE
8. Implement processor initialize - DONE
9. Implement yield
10. Implement scheduler initialize using thrd_t - ON-GOING
11. Implement `ScorpionDo(sched *self, func, void *arg)` which adds the task to a waiting queue
12. Implement scheduler dispatcher loop that dispatches the waiting queue to processors
13. Implement processor blocked-threads yielding
14. Wire-up the _handleTaskStack using SIGSEGV/SIGBUS signal listeners
15. Implement processor next-task algorithm based in round-robin
