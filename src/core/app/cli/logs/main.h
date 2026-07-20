//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <stdio.h>

struct LoggerPlain {
    void (*warn)(const char *);
    void (*info)(const char *);
    void (*error)(const char *);
    void (*debug)(const char *);
    void (*print)(const char *);
};

struct LoggerNewLine {
    void (*warnln)(const char *);
    void (*infoln)(const char *);
    void (*errorln)(const char *);
    void (*debugln)(const char *);
    void (*println)(const char *);
};

struct LoggerFormat {
    void (*warnf)(const char *fmt, ...);
    void (*infof)(const char *fmt, ...);
    void (*errorf)(const char *fmt, ...);
    void (*debugf)(const char *fmt, ...);
    void (*printf)(const char *fmt, ...);

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
    void (*bytesf)(const char *fmt, ...);
};

struct LoggerFile {
    void (*fwarnf)(FILE *file, const char *fmt, ...);
    void (*finfof)(FILE *file, const char *fmt, ...);
    void (*ferrorf)(FILE *file, const char *fmt, ...);
    void (*fdebugf)(FILE *file, const char *fmt, ...);
    void (*fprintf)(FILE *file, const char *fmt, ...);
};

typedef struct {
    struct LoggerPlain plain;
    struct LoggerNewLine newLine;
    struct LoggerFormat fmt;
    struct LoggerFile file;
} LoggerInstance;

extern const LoggerInstance Logger;
