//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "../../shared/types/main.h"
#include <stdio.h>

typedef struct {    
    /*
     * Return rules:
     *     0+:  Everything right
     *     -1:  Can't open the file
     */
    int (*countLines)(string_t path);
    
    /*
     * Return rules:
     *      0:  Everything right
     *     -1:  Line exceed file lines
     *      1:  Can't open the file
     * 
     * out values:
     *      b = NULL: Line empty
     */
    int (*getLine)(string_t path, int line, bytes_t *out);
    
    /*
     * Return rules:
     *      0:  Everything right
     *      1:  Can't open the file
     * 
     * out values:
     *      b = NULL: Format don't found
     */
    int (*getFmt)(string_t path, bytes_t *fmt);
    
    /*
     * Return rules:
     *      0: Everything right
     *     -1: Can't read the file
     *      1: Can't open the file
     * 
     * out values:
     *      b = NULL: File empty
     */
     int (*readFIle)(string_t path, bytes_t *out);
    
    /*
     * Return rules:
     *      0: Everything right
     *     -1: Can't write to file
     *      1: Can't open the file
     */
     int (*writeFile)(string_t path, bytes_t *src);
    
    /*
     * Return rules:
     *      0: Everything right
     *      1: Can't open the file
     */
    int (*appendFile)(string_t path, bytes_t *src);
    
    /*
     * Return rules:
     *      0: Everything right
     *     -1: Directory or file with same name in same path already exists
     *      1: Can't create directory
     */
    int (*makeDir)(string_t path);
    
    /*
     * Return rules:
     *      0: Everything right
     *     -1: Directory do not exists
     *      1: Can't delete directory
     */
    int (*rmDir)(string_t path);

} FsInstance;

extern FsInstance Files;