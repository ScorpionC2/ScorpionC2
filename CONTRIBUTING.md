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

For code contributions (bug fixes, features, refactors or anything that needs code) you'll need a
local copy of the project, in sync with your GitHub account. Follow these steps to get one working:

1. **_Fork the repository_**: Go to https://github.com/ScorpionC2/ScorpionC2/fork
2. **_Clone your fork locally_**
3. **_Install the dependencies_**: The build and testing targets need the following tools:

   - `gcc` — the C compiler
   - `nasm` — the assembler, required to build the scheduler context (`src/core/infra/sched/context/main.s`)
   - `valgrind` — required by `make run-valgrind`
   - `make` — the build tool

4. **_Code your implementation_**
5. **_Test it_**: You can test every new implementation with:

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

Create a new branch from `dev` with a descriptive name.

Examples:

feat/issue-X<br/>
fix/issue-X<br/>
ref/issue-X<br/>

Example command:

```bash
git checkout dev
git checkout -b feat/issue-X
````

### 3. Implement your changes

While implementing your code:

- Follow the existing code style (see [Code Style](#code-style))
- Keep functions small and readable
- Avoid unnecessary dependencies
- Write clear comments when logic is complex
- Document public functions and structs with Doxygen comments (`@brief`, `@param`, `@return`, `@note`, `@pre`, `@post`)
- Register any new source file in the `Makefile`:
  - add the module `.c` files to `CORE_SRC`, and to `TEST_SRC` if you wrote test files;
  - add the new include directory to `I_FLAGS` (and `TEST_I_FLAGS` if you wrote test files).
  If you skip this, the CI and the local targets won't compile your module.

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

The internal test engine (`src/tests/engine`) is minimal and low-level by design. You must use it to
test your code.

How it works:

- Tests are `bool_t` functions that return `TRUE` on success
- Tests are registered automatically at the bottom of the test file with
  `TESTS_UNIT_REGISTER(func, "name")` (constructor-based registration)
- Assertions use the engine macros: `TEST_EQUAL`, `TEST_UNEQUAL`, `TEST_BIGGER`, `TEST_LOWER`,
  `TEST_BIGGER_EQUAL`, `TEST_LOWER_EQUAL`, `TEST_EQUAL_BYTES`, `TEST_UNEQUAL_BYTES`
- `make test` compiles with AddressSanitizer (`-fsanitize=address`), so any memory error stops the whole run
- Keep one test file per module, under `src/tests/unit/<module>/`

Keep in mind:

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
fix(infra/sched/tasks): Implemented task argument passing and corrected stack alignment
feat(infra/sched/queue): Created task queue module
feat(infra/sched/processor): Created scheduler processor for task management
ref(encoders/xor): Removed srcCopy from the xor encoder
test(encoders/xor): Added tests for different hash size configurations
chore(make): Added .c of new features to SRC_ENTRYPOINT
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

The scope is the module you changed (e.g. `infra/sched/tasks`, `app/cli/logs`, `shared/types`,
`encoders/xor`) and it is **mandatory**.

Types used in the project history:

- `feat` — new feature or module
- `fix` — bug fix
- `ref` — refactor that keeps behavior
- `docs` — documentation update only
- `test` — tests only
- `chore` — build, Makefile or CI maintenance
- `style` — formatting or linting only

Examples:

```
feat(infra/sched/queue): Created task queue module
feat(infra/sched/queue): Introduced cursor and implemented task navigation functions
feat(infra/sched/tasks): Implemented task management
feat(infra/sched/tasks/stack): Implemented virtual memory based task stack management
feat(infra/sched/processor): Created scheduler processor for task management
fix(infra/sched/tasks): Implemented task argument passing and corrected stack alignment
docs(infra/sched/context): Documented scheduler context structure and functions
docs(infra/sched/tasks/stack): Corrected return type documentation
docs(todo): Updated status for dynamic stack growing task
ref(encoders/xor): Removed srcCopy from the xor encoder
test(hash/djb2): Added unit tests for DJB2 hash function
chore(Makefile): Added NASM assembly compilation to build targets
chore(make): Added .c of new features to SRC_ENTRYPOINT
```

## Code Style

To keep the codebase consistent and readable, all contributions must follow the project's coding style.

---

### Indentation

- Use **4 spaces** for indentation.
- **Tabs are not allowed out of `Makefile`.**
- Keep indentation consistent across blocks.

Example:

```c
void foo(void bar) {
    return;
}
````

---

### Braces

Opening braces must stay on the **same line** as the statement.

```c
int sum(int a, int b) {
    return a + b;
}
```

This improves readability and visually separates the block ending.

> [!NOTE]
> If a structure (like for, if or while) don't need braces, the project standards ensure the removing of useless braces:
> ```c
> if (x > y)
>     return;
> ```

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

Functions must use **snake_case**.

Internal functions must be prefixed with a single underscore `_`.

```c
struct SchedTask_t *_initTask(void (*func)(void *), void *arg);
void _exitTask();
void *_removeNode(struct SchedQueue_Node_t *node);
```

Function pointer members of module instances keep the **camelCase** name used to call them
(e.g. `Xor->encode()`, `Logger.fmt.warnf()`, `Files.appendFile()`).

---

#### Struct Types

Structs and public types use **PascalCase** and end with the **`_t`** suffix.

```c
typedef struct {
    uint64_t id;
    struct SchedTask_t *task;
    struct SchedQueue_Node_t *prev;
    struct SchedQueue_Node_t *next;
} SchedQueue_Node_t;
```

Nested or derived types keep the `_t` suffix as well: `SchedTask_t`, `SchedTask_Stack_t`,
`SchedQueue_Node_t`. The shared scalar types also follow this rule: `bool_t`, `string_t`, `bytes_t`.

---

#### Module Instances

Public module instances use **PascalCase**.

```c
extern const LoggerInstance Logger;
extern const RandomInstance Random;
extern FsInstance Files;
```

---

#### Macros

Macros must use **UPPER_CASE**.

```c
#define RESET "\x1b[0m"
#define FG_GREEN "\x1b[38;2;72;168;48m"
#define TESTS_UNIT_REGISTER(f, n) __reg_unit_test_##f
```

---

### Pointer Style

The `*` should be attached to the **variable name**, not the type.

```c
char *string;
uint32_t *uintArr;
```

Never do this:

```c
char* string;
uint32* uintArr;
```

---

### Struct Layout

Struct fields should be grouped logically and aligned for readability.

```c
typedef struct {
    uchar_t *buf;
    size_t cap;
    size_t size;
} SchedTask_Stack_t;
```

---

### Function Pointers

Function pointers must be clearly declared inside structs.

```c
typedef struct {
    void (*func)(void *);
    void *arg;
} SchedTask_t;
```

---

### Documentation Comments

Public functions, structs and their fields must be documented with Doxygen-style comments using
`@brief`, `@param`, `@return`, `@retval`, `@note`, `@pre`, `@post` and `@warning`.

Example:

```c
/**
 * @brief Initialize task.
 *
 * @param func The function that will run in the task.
 * @param arg Pointer to the @p func argument.
 *
 * @return Pointer to the new task.
 * @retval NULL The given @p func parameter is NULL.
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
    Logger.newLine.warnln("Can't write last user input to history file");
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
```

---

### Header Files

Header files must:

- use `#pragma once`
- include only necessary dependencies
- expose only public interfaces
- group project includes (e.g. `src/core/...`, `tests/engine/...`) before system includes (`<stdlib.h>`, ...)

## Testing

ScorpionC2 has a custom testing engine (look at [Test your implementation](#4-test-your-implementation))
that you should use to test your custom implementations before sending any Pull Requests.

The engine lives in `src/tests/engine` and unit tests live in `src/tests/unit/<module>/`.

Tests are plain `bool_t` functions that return `TRUE` on success, registered with
`TESTS_UNIT_REGISTER(func, "name")` at the bottom of the file. They are discovered automatically
through constructor registration, and assertions use the engine macros (`TEST_EQUAL`,
`TEST_EQUAL_BYTES`, ...).

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
* unit tests pass (`make test` is compiled with AddressSanitizer)

Pull Requests that introduce compilation errors, crashes, or memory leaks will not be accepted.

## Security Issues

If you discover a security vulnerability, do not open a public issue.

Please follow the responsible disclosure process described in [SECURITY](SECURITY.md) file.

## License

By contributing you agree that your contributions will be licensed under the project license.
