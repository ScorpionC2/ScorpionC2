//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "main.h"
#include "src-server/app/cli/colors/main.h"
#include "src-server/shared/types/main.h"

#include <stdarg.h>
#include <stdio.h>

void warn(const char *self) {
    printf(FG_YELLOW_BOLD "[!]" RESET FG_YELLOW_ITALIC " %s" RESET, self);
}

void info(const char *self) {
    printf(FG_GREEN "[?]" FG_GREEN_ITALIC " %s" RESET, self);
}

void error(const char *self) {
    printf(BG_YELLOW FG_RED_BOLD "[*]" RESET BG_YELLOW FG_RED_ITALIC
                                 " %s" RESET,
           self);
}

void debug(const char *self) {
    printf(FG_GREY_DARK "[DEBUG]" FG_GREY_DARK_ITALIC " %s" RESET, self);
}

void logger_print(const char *self) { printf("%s", self); }

void warnln(const char *self) {
    printf(FG_YELLOW_BOLD "[!]" RESET FG_YELLOW_ITALIC " %s\n" RESET, self);
}

void infoln(const char *self) {
    printf(FG_GREEN "[?]" FG_GREEN_ITALIC " %s\n" RESET, self);
}

void errorln(const char *self) {
    printf(BG_YELLOW FG_RED_BOLD "[*]" RESET BG_YELLOW FG_RED_ITALIC
                                 " %s\n" RESET,
           self);
}

void debugln(const char *self) {
    printf(FG_GREY_DARK "[DEBUG]" FG_GREY_DARK_ITALIC " %s\n" RESET, self);
}

void logger_println(const char *self) { printf("%s\n", self); }

#define _FORMATF_HELPER(name, x)                                               \
    __attribute__((format(printf, 1, 2))) void f_##name(const char *fmt,       \
                                                        ...) {                 \
        va_list arg;                                                           \
        va_start(arg, fmt);                                                    \
                                                                               \
        printf(x);                                                             \
        vprintf(fmt, arg);                                                     \
        printf(RESET);                                                         \
                                                                               \
        va_end(arg);                                                           \
    }

_FORMATF_HELPER(warnf, FG_YELLOW_BOLD "[!]" RESET FG_GREY_DARK_ITALIC " ")
_FORMATF_HELPER(infof, FG_GREEN "[?]" RESET FG_GREEN_ITALIC " ")
_FORMATF_HELPER(errorf, BG_YELLOW_DARK FG_RED_BOLD
                "[*]" RESET BG_YELLOW FG_RED_ITALIC " ")
_FORMATF_HELPER(debugf, FG_GREY_DARK "[DEBUG]" FG_GREY_DARK_ITALIC " ")

__attribute__((format(printf, 1, 2))) void logger_printf(const char *fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    vprintf(fmt, arg);
    va_end(arg);
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
}

#define _F_FORMATF_HELPER(name, x)                                             \
    __attribute__((format(printf, 2, 3))) void f_##name(                       \
        FILE *file, const char *fmt, ...) {                                    \
        va_list arg;                                                           \
        va_start(arg, fmt);                                                    \
                                                                               \
        fprintf(file, x);                                                      \
        vfprintf(file, fmt, arg);                                              \
        fprintf(file, RESET);                                                  \
                                                                               \
        va_end(arg);                                                           \
    }

_F_FORMATF_HELPER(fwarnf, FG_YELLOW_BOLD "[!]" RESET FG_GREY_DARK_ITALIC " ")
_F_FORMATF_HELPER(finfof, FG_GREEN "[?]" RESET FG_GREEN_ITALIC " ")
_F_FORMATF_HELPER(ferrorf, BG_YELLOW_DARK FG_RED_BOLD
                  "[*]" RESET BG_YELLOW FG_RED_ITALIC " ")
_F_FORMATF_HELPER(fdebugf, FG_GREY_DARK "[DEBUG]" FG_GREY_DARK_ITALIC " ")

__attribute__((format(printf, 2, 3))) void
logger_fprintf(FILE *file, const char *fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    vfprintf(file, fmt, arg);
    va_end(arg);
}

const LoggerInstance Logger = {
    .plain =
        {
            .debug = debug,
            .error = error,
            .info = info,
            .print = logger_print,
            .warn = warn,
        },
    .newLine =
        {
            .debugln = debugln,
            .errorln = errorln,
            .infoln = infoln,
            .println = logger_println,
            .warnln = warnln,
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
