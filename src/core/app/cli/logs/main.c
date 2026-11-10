//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "main.h"
#include "src/core/app/cli/colors/main.h"
#include "src/core/shared/types/main.h"

#include <stdarg.h>
#include <stdio.h>

#define WARN_FORMAT FG_YELLOW_BOLD "[!]" RESET FG_GREY_DARK_ITALIC " "
#define INFO_FORMAT FG_GREEN "[?]" RESET FG_GREEN_ITALIC " "
#define DEBUG_FORMAT FG_GREY_DARK "[DEBUG]" FG_GREY_DARK_ITALIC " "
#define ERROR_FORMAT                                                           \
    BG_YELLOW_DARK FG_RED_BOLD "[*]" RESET BG_YELLOW FG_RED_ITALIC " "

#define _PLAIN_HELPER(name, x)                                                 \
    void f_##name(const char *str) { printf(x "%s" RESET, str); }

_PLAIN_HELPER(warn, WARN_FORMAT)
_PLAIN_HELPER(info, INFO_FORMAT)
_PLAIN_HELPER(error, ERROR_FORMAT)
_PLAIN_HELPER(debug, DEBUG_FORMAT)

void logger_print(const char *self) { printf("%s", self); }

#define _LN_HELPER(name, x)                                                    \
    void f_##name(const char *str) {                                           \
        flockfile(stdout);                                                     \
                                                                               \
        printf(x "%s\n" RESET, str);                                           \
                                                                               \
        funlockfile(stdout);                                                   \
    }

_LN_HELPER(warnln, WARN_FORMAT)
_LN_HELPER(infoln, INFO_FORMAT)
_LN_HELPER(errorln, ERROR_FORMAT)
_LN_HELPER(debugln, DEBUG_FORMAT)

void logger_println(const char *self) { printf("%s\n", self); }

#define _FORMATF_HELPER(name, x)                                               \
    __attribute__((format(printf, 1, 2))) void f_##name(const char *fmt,       \
                                                        ...) {                 \
        flockfile(stdout);                                                     \
                                                                               \
        va_list arg;                                                           \
        va_start(arg, fmt);                                                    \
                                                                               \
        printf(x);                                                             \
        vprintf(fmt, arg);                                                     \
        printf(RESET);                                                         \
                                                                               \
        va_end(arg);                                                           \
        funlockfile(stdout);                                                   \
    }

_FORMATF_HELPER(warnf, WARN_FORMAT)
_FORMATF_HELPER(infof, INFO_FORMAT)
_FORMATF_HELPER(errorf, ERROR_FORMAT)
_FORMATF_HELPER(debugf, DEBUG_FORMAT)

__attribute__((format(printf, 1, 2))) void logger_printf(const char *fmt, ...) {
    flockfile(stdout);

    va_list arg;
    va_start(arg, fmt);

    vprintf(fmt, arg);

    va_end(arg);
    funlockfile(stdout);
}

/*
 * bytesf supports:
 *  %b: print array of bytes like [ff, ff, ff, ff]
 * 
 *  %bh: print array of bytes like [0xff, 0xff, 0xff, 0xff]
 * 
 *  %p: print pointer to anything like 0xFFFFFFFFFFFF
 * 
 *  %%: print '%' character
 * 
 * Notes:
 *  %b and %bh wants pointer to bytes_t type.
 *  %p wants pointer to anything (generic void* type)
 */
void bytesf(const char *fmt, ...) {
    flockfile(stdout);
    va_list args;
    va_start(args, fmt);
    for (const char *p = fmt; *p; p++) {
        if (*p != '%') {
            putchar(*p);
            continue;
        }

        switch (*(p + 1)) {
            case 'b': {
                const bytes_t *bytes = va_arg(args, bytes_t *);

                const char _fmtBH[] = "%#x";
                const char _fmtB[] = "%x";

                const char *printfFmt = (const char *)_fmtB;
                if (*(p + 2) == 'h')
                    printfFmt = (const char *)_fmtBH;

                printf("[");
                printf(printfFmt, bytes->b[0]);
                for (size_t i = 1; i < bytes->len; i++) {
                    printf(", %x", bytes->b[i]);
                }

                printf("]");
                fflush(stdout);
                p++;

                continue;
            }

            case 'p': {
                void *ptr = va_arg(args, void *);
                printf("%p", ptr);
                fflush(stdout);
                p++;

                continue;
            }

            case '%': {
                printf("%%");
                fflush(stdout);
                p++;

                continue;
            }

            default:
                break;
        }
    }

    funlockfile(stdout);
}

#define _F_FORMATF_HELPER(name, x)                                             \
    __attribute__((format(printf, 2, 3))) void f_##name(                       \
        FILE *file, const char *fmt, ...) {                                    \
        flockfile(file);                                                       \
                                                                               \
        va_list arg;                                                           \
        va_start(arg, fmt);                                                    \
                                                                               \
        fprintf(file, x);                                                      \
        vfprintf(file, fmt, arg);                                              \
        fprintf(file, RESET);                                                  \
                                                                               \
        va_end(arg);                                                           \
        funlockfile(file);                                                     \
    }

_F_FORMATF_HELPER(fwarnf, WARN_FORMAT)
_F_FORMATF_HELPER(finfof, INFO_FORMAT)
_F_FORMATF_HELPER(ferrorf, ERROR_FORMAT)
_F_FORMATF_HELPER(fdebugf, DEBUG_FORMAT)

__attribute__((format(printf, 2, 3))) void
logger_fprintf(FILE *file, const char *fmt, ...) {
    flockfile(file);

    va_list arg;
    va_start(arg, fmt);

    vfprintf(file, fmt, arg);

    va_end(arg);
    funlockfile(file);
}

const LoggerInstance Logger = {
    .plain =
        {
            .debug = f_debug,
            .error = f_error,
            .info = f_info,
            .print = logger_print,
            .warn = f_warn,
        },
    .newLine =
        {
            .debugln = f_debugln,
            .errorln = f_errorln,
            .infoln = f_infoln,
            .println = logger_println,
            .warnln = f_warnln,
        },
    .fmt =
        {
            .bytesf = bytesf,
            .debugf = f_debugf,
            .errorf = f_errorf,
            .infof = f_infof,
            .printf = logger_printf,
            .warnf = f_warnf,
        },
    .file = {.fdebugf = f_fdebugf,
             .ferrorf = f_ferrorf,
             .finfof = f_finfof,
             .fprintf = logger_fprintf,
             .fwarnf = f_fwarnf},
};
