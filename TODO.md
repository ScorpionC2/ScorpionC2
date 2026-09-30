# ScorpionC2 TODO List

## Issues

## **Features Backlog**

1. Create SocorpionDo (go-like fast and lightweight coroutines) ON-GOING
2. Create ScorpionSk (socket-like Parallel and Concurrent API)
3. Create SDCS (Secure Dissemination of Cryptographic Secrets) using an Encoder over TLS distribute cryptographic keys for Custom Secure Protocols

## **Tasks, Refactors and Improvements**

1. Hash Testing Suite https://github.com/ScorpionC2/ScorpionC2/issues/70
2. Randomize the xor encoder trash size https://github.com/ScorpionC2/ScorpionC2/issues/71


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
9. Implement processor loop - DONE
10. Implement yield
11. Implement processor blocked-threads yielding
12. Implement processor next-task algorithm based in round-robin - DONE (just `cursor->next` yet)
13. Implement scheduler initialize
14. Implement scheduler loop
