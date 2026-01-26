//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <stdio.h>

typedef struct {
    void (*warn)(const char*);
    void (*info)(const char*);
    void (*error)(const char*);
    void (*debug)(const char*);
    void (*print)(const char*);
    
    void (*warnln)(const char*);
    void (*infoln)(const char*);
    void (*errorln)(const char*);
    void (*debugln)(const char*);
    void (*println)(const char*);
    
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
     */
    void (*bytesf)(const char *fmt, ...);
    
    void (*fwarnf)(FILE *file, const char *fmt, ...);
    void (*finfof)(FILE *file, const char *fmt, ...);
    void (*ferrorf)(FILE *file, const char *fmt, ...);
    void (*fdebugf)(FILE *file, const char *fmt, ...);
    void (*fprintf)(FILE *file, const char *fmt, ...);
    
} LoggerInstance;

extern const LoggerInstance Logger;