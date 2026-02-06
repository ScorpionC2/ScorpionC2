//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
// Implementation is based in the following contents:
//  https://www.reddit.com/r/C_Programming/comments/1go5ee2/how_do_you_get_user_input_safely/
//  https://www.ibm.com/docs/pt-br/i/7.6.0?topic=functions-fgets-read-string
//  https://github.com/Yyax13/TheDarkMark/blob/bdc5dca9135d6601af60287a6ef9580d014444fe/src/spells/x86linux/basic/reverse_shell/main.c#L702
//  https://en.wikipedia.org/wiki/ANSI_escape_code
//  https://stackoverflow.com/questions/7469139/what-is-the-equivalent-to-getch-getche-in-linux
//  https://www.ibm.com/docs/pt-br/i/7.6.0?topic=functions-fgets-read-string
// 
//  TODO List:
//      DONE  - Implement custom getchar without ECHO and CANONICAL modes
//      INDEV - Implement custom read nor fgets above custom getchar
//      TODO  - Adapt custom read/fgets to snread

#include "main.h"
#include "../readMode/main.h"
#include "../../../../infra/fs/main.h"
#include "../../logs/main.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// ucgetchar - Ultra-incredible-handmade galactic empire engineer's Custom GETCHAR
char ucgetchar(FILE *stream) {
    TerminalModeToggler.turnCanonicalMode(FALSE);
    TerminalModeToggler.turnEchoMode(FALSE);
    
    char ch = getchar();
    
    return ch;
    
    TerminalModeToggler.turnCanonicalMode(TRUE);
    TerminalModeToggler.turnEchoMode(TRUE);
    
};

// ucfgets - Ultra-incredible-handmade galactic empire engineer's Custom FGETS
char* ucfgets(char *dest, string_t histPath, int promptSize) {
    size_t bufCap = 65536;
    size_t bufLen = 0;
    char *buf = malloc(bufCap);
    if (buf == NULL) {
        return NULL;
        
    }
    
    char ch;
    long _n;
    
    int currentHistLine;
    int histFileLines = Files.countLines(histPath);
    if (histFileLines < 0) {
        return NULL;
        
    }
    
    currentHistLine = histFileLines;

    for (int i = 0; (ch = ucgetchar(stdin)) != '\n'; i++) {
        if ((bufLen + 1 + _n) > bufCap) {
            while ((bufLen + 1 + _n) > bufCap) {
                bufCap <<= 1;
                
            }
            
            char *newBuf = realloc(buf, bufCap);
            if (newBuf == NULL) {
                free(buf);
                return NULL;
                
            }
            
            buf = newBuf;
            
        }
        
        if (ch == 0x1b) {
            char ch2 = ucgetchar(stdin);
            i++;
            
            if (ch2 == 0x5b) {
                char ch3 = ucgetchar(stdin);
                i++;
                
                bytes_t l;
                char *histLine;
                switch (ch3) {
                    case 0x41: // Arrow up
                        if (Files.getLine(histPath, currentHistLine + 1, &l) != 0) {
                            Logger.warnln("Can't parse hist line");
                            continue;
                            
                        }
                        
                        histLine = malloc(l.len);
                        if (histLine == NULL) {
                            Logger.warnln("Can't malloc");
                            continue;

                        }

                        memcpy(histLine, l.b, l.len);
                        histLine[strcspn(histLine, "\n")] = '\0';
                        
                        printf("\x1b[1G\x1b[%dC%s", promptSize, histLine);
                        
                        free(histLine);
                        free(l.b);
                        currentHistLine--;
                        
                        continue;
                    
                    case 0x42:
                        if (Files.getLine(histPath, currentHistLine - 1, &l) != 0) {
                            Logger.warnln("Can't parse hist line");
                            continue;
                            
                        }
                        
                        histLine = malloc(l.len);
                        if (histLine == NULL) {
                            Logger.warnln("Can't malloc");
                            continue;
                            
                        }
                        
                        memcpy(histLine, l.b, l.len);
                        histLine[strcspn(histLine, "\n")] = '\0';
                        
                        printf("\x1b[1G\x1b[%dC%s", promptSize, histLine);
                        
                        free(histLine);
                        free(l.b);
                        currentHistLine--;
                        
                        continue;
                        
                    default:
                        continue;

                }
            }
        } else {
            buf[i] = ch;
            
        }
        
        _n++;
        bufLen += _n;
        
    }
    
    dest = malloc(bufLen);
    memcpy(dest, buf, bufLen);
    
    free(buf);
    return dest;
    
};

string_t ucread(string_t histPath, int promptSize) {
    string_t out;
    char *buf;
    
    if (ucfgets(buf, histPath, promptSize) == NULL) {
        out.len = 0;
        out.s = NULL;
        return out;
        
    }
    
    buf[strcspn(buf, "\n")] = '\0';
    out.len = strlen(buf);
    out.s = malloc(out.len);
    if (out.s == NULL) {
        out.len = 0;
        return out;
        
    }
    
    memcpy(out.s, buf, out.len);
    free(buf);
    
    return out;
    
};

SafeGetInstance SafeGet = {
    .ucread = ucread,
    .ucfgets = ucfgets
    
};