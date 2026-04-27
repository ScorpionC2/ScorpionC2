# Contributing

Wow! Thank you very much for writing code to this project!<br/>
This file explain how your contribution can be accepted in our source-code.

## Ways to contribute

Code isn't the unique way of contribution, in this section we'll overview every contribution way that you should do.

### Reporting Bugs

Bugs are really easy to report, you just need to open an issue with the _Bug Report_ template (e.g 
[#23](https://github.com/ScorpionC2/ScorpionC2/issues/23))

Your issue must have all this fields:

1. **_Bug Description_**: Describe the bug, how it happens and how you find it
2. **_Steps to reproduce_**: Describe how the maintainer can recreate the bug
3. **_Expected behavior_**: Explain how the program should run without the bug
4. **_Actual behavior_**: Explain what is happening with the bug
5. **_Logs / stack traces_**: Logs and stack traces of the program during the bug execution
6. **_Severity_**: Should be `low`, `medium`, `high` or `critical`
7. **_Version or commit hash_**: The hash of the commit or released version
8. **_Environment_**: Kernel version, GCC version, valgrind version, etc
9. **_Confirmation_**: Check the boxes confirming that you already checked for similar issues and can reproduce the bug consistently

After that a maintainer will assign to your issue and solve it.<br/>
Feel free to solve your own issue and open a PR!

### Feature request

Features requests are easy to submit, you just need to open an issue with the _Feature Request_ template (e.g.
[#17](https://github.com/ScorpionC2/ScorpionC2/issues/17))

Your issue must have all this fields:

1. **_Feature Summary_**: Short description of the feature
2. **_Problem or motivation_**: Why this feature is useful
3. **_Proposed Solution_**: How to solve the problem/motivation
4. **_Alternatives Considered_**: Any other alternatives for solving the same problem
5. **_Impact_**: What, in the project, will change with this feature
6. **_Priority_**: How faster this problem needs to be solved with that one feature
7. **_Scope_**: Estimated impact on the codebase
8. **_Related version or commit_**: Version or commit where this feature would be relevant
9. **_Confirmation_**: Check the boxes confirming that you already checked for similar issues and 
if this feature aligns with project's goals

You can implement your own feature request, feel free for doing that and opening a PR!

## Development Setup

For developing your contribution (if is a code contribution, bug fix nor anything that 
need code) you may need a local version of the project, follow this steps to get one working
sync with your GitHub account.

1. **_Fork the repository_**: Go to https://github.com/ScorpionC2/ScorpionC2/fork
2. **_Clone your fork locally_**
3. **_Code your implementation_**
4. **_Test it_**: You can test every new implementation with:

```shell
make run # Run the project in /tmp
make run-valgrind # Run the project in /tmp with valgrind
make run-debug # Run the project in /tmp with GCC debug flags
make test # Run tests
```

## Pull Request Workflow

Pull Requests (PRs) are the main way to contribute code to the project.

Follow these steps before submitting your PR.

### 1. Ensure the issue exists

Before writing code, make sure there is an **open issue** describing the bug or feature.

- If it does not exist, create one first.
- Reference the issue in your PR.

Example:

Closes #23

### 2. Create a branch

Create a new branch from `main` with a descriptive name.

Examples:

feat/issue-X<br/>
fix/issue-X<br/>
ref/issue-X<br/>

Example command:

```bash
git checkout -b feat/issue-X
````

### 3. Implement your changes

While implementing your code:

- Follow the existing code style
- Keep functions small and readable
- Avoid unnecessary dependencies
- Write clear comments when logic is complex

<!-- TOC --><a name="4-test-your-implementation"></a>
### 4. Test your implementation

All new code must be covered by unit tests.

- Features must include new tests
- Bug fixes must include a test reproducing the issue
- Refactors must not break existing tests

Pull Requests without tests may be rejected.

Before opening a PR, make sure that your code runs with

```bash
make run
make run-valgrind
make run-debug
make test
```

Your code **must not introduce memory leaks or crashes**.

### 4.5 Test Engine Guidelines

The internal test engine is minimal and low-level by design.

Keep in mind:

- Tests should stop if memory errors happens
- Tests are executed automatically via constructor registration
- Avoid global state when writing tests
- Do not rely on execution order between tests
- Keep assertions explicit and simple
- Prefer multiple small tests over one large test

Bad example:

- One test validating multiple unrelated behaviors
- One file validating more than one module, service, infrastructure abstraction or app module

Good example:

- One test per function or edge case
- One file that validates one module, service, infrastructure abstraction or app module

### 5. Commit your changes

Write clear and descriptive commit messages.

Recommended format:

```
type(scope): Short description starting in past-tense
```

Examples:

```
fix(infra/parser): Resolved buffer overflow
feat(protocols/sctcp): Added encrypted session handshake
ref(domain/protocols): Simplified socket handling
```

### 6. Push and open the Pull Request

Push your branch and open a PR against `dev`.

Example:

```bash
git push origin feat/issue-X # For features
git push origin fix/issue-X # For bug fixes
```

Your PR must:

* Describe the change clearly
* Reference related issues
* Explain why the change is necessary

### 7. Code Review

A maintainer will review your PR.

Possible outcomes:

- Accepted and merged
- Changes requested
- Rejected (with explanation)

Please be patient during review and respond to feedback.


### 8. Merge

Once approved, a maintainer will merge the PR into `dev`, and later, he shall merge the PR into `main`.

## Commit Convention

Our commit convention will be explained down here:

```
<type>(<non-optional-scope>): Message started with past-tense verb
```

Examples directly from git history:

```
feat(encoders/xor): Added settings option to use different hash algorithms
feat(app/main): Added tests for the new hashing algorithms
feat(utils/hash): Created custom hashing
fix(utils/random): Fixed modulo bias in randr using limit and rejecting trash values; Refactored seed_g incrementation;
ref(cli/loading): Deleted useless cli/loading modules
feat(app/main): Added tests for the new hash module and new xor encoder
chore(make): Added .c of new features to SRC_ENTRYPOINT
feat(encoders/xor): Created xor encoder and updated types
feat(shared/types): Added new types: bool_t and ulong_t
feat(utils/hash): Implemented hashing and djb2 hash
docs(cli/logs): Updated docs for bytesf
fix(utils/random): Fixed incorrect include for unistd.h
feat(app/main): Added tests for the new random module
feat(utils/random): Created pseudo-random numbers generation (a.k.a. RNG)
feat(domain/encoders): Created main.h with base docs to define the project default architecture for encoders
```

## Code Style

To keep the codebase consistent and readable, all contributions must follow the project's coding style.

---

### Indentation

- Use **4 spaces** for indentation.
- **Tabs are not allowed out of a `Makefile`.**
- Keep indentation consistent across blocks.

Example:

```c
if (condition) {
    doSomething();
}
````

---

### Braces

Opening braces must stay on the **same line** as the statement.

```c
if (x > y) {
    doSomething();
}
```

This improves readability and visually separates the block ending.

---

### Naming Conventions

#### Variables

Variables must use **camelCase**.

```c
int modY;
string_t userInput;
bytes_t inputRaw;
```

---

#### Functions

Functions must also use **camelCase**.

```c
void readLine();
uint32_t randomSeed();
```

---

#### Struct Types

Structs and public types use **PascalCase**.

```c
typedef struct {
    string_t prompt;
    string_t histPath;
    int histLimit;
} InputSettings;
```

---

#### Module Instances

Public module instances use **PascalCase**.

```c
InputInstance Input;
```

---

#### Macros

Macros must use **UPPER_CASE**.

```c
#define RESET "\x1b[0m"
#define FG_GREEN "\x1b[38;2;72;168;48m"
```

---

### Pointer Style

The `*` should be attached to the **variable name**, not the type.

```c
char *string;
uint32_t *uintArr;
```

---

### Struct Layout

Struct fields should be grouped logically and aligned for readability.

```c
typedef struct {
    string_t prompt;
    string_t histPath;
    int histLimit;
} InputSettings;
```

---

### Function Pointers

Function pointers must be clearly declared inside structs.

```c
typedef struct {
    void (*readline)(InputSettings conf, string_t *out);
} InputInstance;
```

---

### Memory Management

Always ensure correct allocation size and free allocated memory when no longer needed.

Example:

```c
out->s = malloc(out->len + 1);
memcpy(out->s, input.s, out->len);
out->s[out->len] = '\0';
```

---

### Error Handling

Always check return values for operations involving:

* file I/O
* memory allocation
* system calls

Example:

```c
if (Files.appendFile(histPath, &inputRaw) != 0) {
    Logger.warnln("Can't write last user input to history file");
}
```

---

### File Headers

Every file must start with the project license header.

```c
//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//
// Contribution formal owner is <"Your full name here" or Anonymous>. All rights reserved.
// This snippet of code stands under the MIT license, as the entire project. See LICENSE file in the project root for details.
//
```

---

### Header Files

Header files must:

- use `#pragma once`
- include only necessary dependencies
- expose only public interfaces

## Testing

ScorpionC2 have a custom testing engine (look at [Test your implementation](#4-test-your-implementation)), that you should use to test your custom implementations before sending any Pull Requests

Before submitting a Pull Request, contributors must ensure that the code compiles
and runs correctly using the provided Make targets.

Run the following commands:

```bash
make run
make run-valgrind
make run-debug
make test
````

These commands validate that:

* the project compiles successfully
* the program runs correctly
* no memory leaks are detected with valgrind
* debug builds compile correctly

Pull Requests that introduce compilation errors, crashes, or memory leaks will not be accepted.

## Security Issues

If you discover a security vulnerability, do not open a public issue.

Please follow the responsible disclosure process described in [SECURITY](SECURITY.md) file.

## License

By contributing you agree that your contributions will be licensed under the project license.

