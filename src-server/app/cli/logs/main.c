//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "main.h"
#include "../colors/main.h"
#include "../../../shared/types/main.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

void warn(const char *self) {
    printf(FG_YELLOW_BOLD "[!]" RESET FG_YELLOW_ITALIC " %s" RESET, self);

}

void info(const char *self) {
    printf(FG_GREEN "[?]" FG_GREEN_ITALIC " %s" RESET, self);

}

void error(const char *self) {
    printf(BG_YELLOW FG_RED_BOLD "[*]" RESET BG_YELLOW FG_RED_ITALIC " %s" RESET, self);

}

void debug(const char *self) {
    printf(FG_GREY_DARK "[DEBUG]" FG_GREY_DARK_ITALIC " %s" RESET, self);
    
}

void logger_print(const char *self) {
    printf("%s", self);

}

void warnln(const char *self) {
    printf(FG_YELLOW_BOLD "[!]" RESET FG_YELLOW_ITALIC " %s\n" RESET, self);

}

void infoln(const char *self) {
    printf(FG_GREEN "[?]" FG_GREEN_ITALIC " %s\n" RESET, self);

}

void errorln(const char *self) {
    printf(BG_YELLOW FG_RED_BOLD "[*]" RESET BG_YELLOW FG_RED_ITALIC " %s\n" RESET, self);

}

void debugln(const char *self) {
    printf(FG_GREY_DARK "[DEBUG]" FG_GREY_DARK_ITALIC " %s\n" RESET, self);
    
}

void logger_println(const char *self) {
    printf("%s\n", self);

}

void warnf(const char *fmt, ...) {
    char *buf = malloc(strlen(fmt) * 4);
    if (buf == NULL) {
        return;
        
    };
    
    va_list arg;
    va_start(arg, fmt);
    vsprintf(buf, fmt, arg);
    va_end(arg);
    
    printf(FG_YELLOW_BOLD "[!]" RESET FG_YELLOW_ITALIC " %s" RESET, buf);
    free(buf);

}

void infof(const char *fmt, ...) {
    char *buf = malloc(strlen(fmt) * 4);
    if (buf == NULL) {
        return;
        
    };
    
    va_list arg;
    va_start(arg, fmt);
    vsprintf(buf, fmt, arg);
    va_end(arg);
    
    printf(FG_GREEN "[?]" FG_GREEN_ITALIC " %s" RESET, buf);
    free(buf);

}

void errorf(const char *fmt, ...) {
    char *buf = malloc(strlen(fmt) * 4);
    if (buf == NULL) {
        return;
        
    };
    
    va_list arg;
    va_start(arg, fmt);
    vsprintf(buf, fmt, arg);
    va_end(arg);
    
    printf(BG_YELLOW FG_RED_BOLD "[*]" RESET BG_YELLOW FG_RED_ITALIC " %s" RESET, buf);
    free(buf);

}

void debugf(const char *fmt, ...) {
    char *buf = malloc(strlen(fmt) * 4);
    if (buf == NULL) {
        return;
        
    };
    
    va_list arg;
    va_start(arg, fmt);
    vsprintf(buf, fmt, arg);
    va_end(arg);
    
    printf(FG_GREY_DARK "[DEBUG]" FG_GREY_DARK_ITALIC " %s" RESET, buf);
    free(buf);
    
}

void logger_printf(const char *fmt, ...) {
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
        
        if (*(p + 1) == 'b') {
            bytes_t *bytes = va_arg(args, bytes_t*);
            if (*(p + 2) == 'h') {
                printf("[%#x", bytes->b[0]);
                for (int i = 1; i < bytes->len; i++) {
                    printf(", %#x", bytes->b[i]);
                    
                }
                
                printf("]");
                fflush(stdout);
                p += 2;
                continue;
                
            }
            
            
            printf("[%x", bytes->b[0]);
            for (int i = 1; i < bytes->len; i++) {
                printf(", %x", bytes->b[i]);
                
            }
            
            printf("]");
            fflush(stdout);
            p++;
            continue;
            
        } else if (*(p + 1) == 'p') {
            void *ptr = va_arg(args, void*);
            printf("%p", ptr);
            fflush(stdout);
            p++;
            continue;
            
        } else if (*(p + 1) == '%') {
            printf("%%");
            fflush(stdout);
            p++;
            continue;
            
        }
        
    }
    
}

void fwarnf(FILE *file, const char *fmt, ...) {
    char *buf = malloc(strlen(fmt) * 4);
    if (buf == NULL) {
        return;
        
    };
    
    va_list arg;
    va_start(arg, fmt);
    vsprintf(buf, fmt, arg);
    va_end(arg);
    
    fprintf(file, FG_YELLOW_BOLD "[!]" RESET FG_YELLOW_ITALIC " %s" RESET, buf);
    free(buf);

}

void finfof(FILE *file, const char *fmt, ...) {
    char *buf = malloc(strlen(fmt) * 4);
    if (buf == NULL) {
        return;
        
    };
    
    va_list arg;
    va_start(arg, fmt);
    vsprintf(buf, fmt, arg);
    va_end(arg);
    
    fprintf(file, FG_GREEN "[?]" FG_GREEN_ITALIC " %s" RESET, buf);
    free(buf);

}

void ferrorf(FILE *file, const char *fmt, ...) {
    char *buf = malloc(strlen(fmt) * 4);
    if (buf == NULL) {
        return;
        
    };
    
    va_list arg;
    va_start(arg, fmt);
    vsprintf(buf, fmt, arg);
    va_end(arg);
    
    fprintf(file, BG_YELLOW FG_RED_BOLD "[*]" RESET BG_YELLOW FG_RED_ITALIC " %s" RESET, buf);
    free(buf);

}

void fdebugf(FILE *file, const char *fmt, ...) {
    char *buf = malloc(strlen(fmt) * 4);
    if (buf == NULL) {
        return;
        
    };
    
    va_list arg;
    va_start(arg, fmt);
    vsprintf(buf, fmt, arg);
    va_end(arg);
    
    fprintf(file, FG_GREY_DARK "[DEBUG]" FG_GREY_DARK_ITALIC " %s" RESET, buf);
    free(buf);
    
}

void logger_fprintf(FILE *file, const char *fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    vfprintf(file, fmt, arg);
    va_end(arg);

}

const LoggerInstance Logger = {
    .debug = debug,
    .warn = warn,
    .info = info,
    .error = error,
    .print = logger_print,
    .debugln = debugln,
    .warnln = warnln,
    .infoln = infoln,
    .errorln = errorln,
    .println = logger_println,
    .debugf = debugf,
    .warnf = warnf,
    .infof = infof,
    .errorf = errorf,
    .printf = logger_printf,
    .bytesf = bytesf,
    .fdebugf = fdebugf,
    .fwarnf = fwarnf,
    .finfof = finfof,
    .ferrorf = ferrorf,
    .fprintf = logger_fprintf
    
};