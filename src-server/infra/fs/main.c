//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int appendFile(string_t path, bytes_t *src) {
    if (!path.s || !path.len) return 1;
    FILE *file = fopen(path.s, "ab+");
    if (file == NULL) {
        return -1;
        
    }
    
    fwrite(src->b, sizeof(src->b[0]), src->len, file);
    
    fclose(file);
    return 0;
    
};

int countLines(string_t path) {
    int count;
    int c;
    if (!path.s || !path.len) {
        return -1;
        
    }
    
    FILE *file = fopen(path.s, "r");
    if (file == NULL) {
        return -1;
        
    }
    
    for (c = fgetc(file); c != EOF; c = fgetc(file)) {
        if (c == '\n') {
            count++;
            
        }
        
    }
    
    fclose(file);
    return count;
    
}

int getLine(string_t path, int line, bytes_t *out) {
    if (!path.s || !path.len) return 1;
    int lines = countLines(path);
    if (lines < 0) {
        return 1;
        
    }
    
    if (line > lines) {
        return -1;
        
    }
    
    FILE *file = fopen(path.s, "r");
    if (file == NULL) {
        return 1;
        
    }
    
    int count = 1;
    char lineBuf[1024];
    while (fgets(lineBuf, sizeof(lineBuf), file) != NULL) {
        if (count == line) {
            break;
            
        }
        
        count++;
        
    }
    
    fclose(file);
    
    lineBuf[strcspn(lineBuf, "\n")] = '\0';
    out->len = strlen(lineBuf) + 1;
    out->b = malloc(out->len);
    memcpy(out->b, lineBuf, out->len);
    
    return 0;
    
};

FsInstance Files = {
    .countLines = countLines,
    .appendFile = appendFile,
    // .getLine = 
    
};