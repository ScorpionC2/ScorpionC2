# ScorpionC2 TODO List

## Improvements

1. Add blocking I/O handling to ScorpionDo Task Scheduler;<br/>
    1.1. Implement yield;<br/>
    1.2. Implement processor blocked-threads yielding;<br/>
    1.3. Wire-up the _handleTaskStack using SIGSEGV/SIGBUS signal listeners;<br/>
    1.4. Implement processor next-task algorithm based in round-robin.<br/>
2. Add variable trash size to xor (https://github.com/ScorpionC2/ScorpionC2/issues/71);
3. Documents everything (https://github.com/ScorpionC2/ScorpionC2/issues/78).

## **Tasks and Features Backlog**

1. Create ScorpionSk (socket-like Parallel and Concurrent API);
2. Create SDCS (Secure Dissemination of Cryptographic Secrets) using an Encoder over TLS distribute cryptographic keys for Custom Secure Protocols.

## Testing

1. Write tests to hashing algorithms (check https://github.com/ScorpionC2/ScorpionC2/issues/70 for more information).
