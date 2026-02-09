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

#include "main.h"
#include "../readMode/main.h"
#include "../../../../infra/fs/main.h"
#include "../../logs/main.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// ucfgets - Ultra-incredible-handmade galactic empire engineer's Custom FGETS
char* ucfgets(string_t histPath, int promptSize) {
    TerminalModeToggler.turnCanonicalMode(FALSE);
    TerminalModeToggler.turnEchoMode(FALSE);
    
    size_t bufCap = 65536;
    size_t bufLen = 0;
    char *buf = malloc(bufCap);
    if (buf == NULL) {
        goto cleanup_error;
        
    }
    
    char ch = '\0';
    
    int currentHistLine;
    int histFileLines = Files.countLines(histPath);
    if (histFileLines < 0) {
        goto cleanup_error;
        
    }
    
    currentHistLine = histFileLines;

    while (TRUE) {
        ssize_t r = read(STDIN_FILENO, &ch, 1);
        if (r <= 0) {
            goto cleanup_error;
            
        }
        
        if (ch == 0xa) {
            putchar(0xa);
            goto cleanup;
            
        }
        
        if ((bufLen + 1) >= bufCap) {
            while ((bufLen + 1) >= bufCap) {
                bufCap <<= 1;
                
            }
            
            char *newBuf = realloc(buf, bufCap);
            if (newBuf == NULL) {
                free(buf);
                goto cleanup_error;
                
            }
            
            buf = newBuf;
            
        }
        
        if (ch == 0x1b) {
            char ch2 = '\0';
            ssize_t r2 = read(STDIN_FILENO, &ch2, 1);
            if (r2 <= 0) {
                goto cleanup_error;
                
            }
            
            if (ch2 == 0x5b) {
                char ch3 = '\0';
                ssize_t r3 = read(STDIN_FILENO, &ch3, 1);
                if (r3 <= 0) {
                    goto cleanup_error;
                    
                }
                
                bytes_t l;
                char *histLine = NULL;
                switch (ch3) {
                    case 0x41: // Arrow up
                        if (histFileLines == 0) continue;
                        if (Files.getLine(histPath, (currentHistLine - 1 + histFileLines) % histFileLines, &l) != 0) {
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
                        free(l.b);
                        
                        bufLen = strlen(histLine);
                        if (bufLen + 1 >= bufCap) {
                            while (bufLen + 1 >= bufCap) {
                                bufCap <<= 1;
                            }
                            buf = realloc(buf, bufCap);
                        }
                        
                        memcpy(buf, histLine, bufLen);
                        buf[bufLen] = '\0';
                        free(histLine);
                        
                        printf("\x1b[1G\x1b[%dC%s\x1b[K", promptSize, buf);
                        fflush(stdout);
                        currentHistLine--;
                        
                        continue;
                    
                    case 0x42:
                        if (histFileLines == 0) continue;

                        if (Files.getLine(histPath, (currentHistLine + 1) % histFileLines, &l) != 0) {
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
                        free(l.b);
                        
                        
                        bufLen = strlen(histLine);
                        if (bufLen + 1 >= bufCap) {
                            while (bufLen + 1 >= bufCap) {
                                bufCap <<= 1;
                            }
                            buf = realloc(buf, bufCap);
                        }
                        
                        memcpy(buf, histLine, bufLen);
                        buf[bufLen] = '\0';
                        free(histLine);
                        
                        printf("\x1b[1G\x1b[%dC%s\x1b[K", promptSize, buf);
                        fflush(stdout);
                        
                        currentHistLine++;
                        
                        continue;
                        
                    default:
                        continue;

                }
            }
        } else if (ch == 0x7f || ch == 0x08) {
            if (bufLen > 0) {
                bufLen--;
                buf[bufLen] = '\0';
            
            }
            
            printf("\x1b[1G\x1b[%dC%s\x1b[K", promptSize, buf);
            fflush(stdout);
            continue;
            
        } else {
            buf[bufLen++] = ch;
            buf[bufLen] = '\0';
            putc(ch, stdout);
            fflush(stdout);
            
        }
        
    }
    
    buf[bufLen] = '\0';
    
    printf("\x1b[1G\x1b[%dC%s\x1b[K", promptSize, buf);
    fflush(stdout);
    
    cleanup:
        TerminalModeToggler.turnCanonicalMode(TRUE);
        TerminalModeToggler.turnEchoMode(TRUE);
        return buf;
    
    cleanup_error:
        TerminalModeToggler.turnCanonicalMode(TRUE);
        TerminalModeToggler.turnEchoMode(TRUE);
        free(buf);
        return NULL;
    
};

string_t ucread(string_t histPath, int promptSize) {
    string_t out;
    char *buf = ucfgets(histPath, promptSize);
    
    if (buf == NULL) {
        out.len = 0;
        out.s = NULL;
        return out;
        
    }
    
    out.len = strlen(buf);
    out.s = malloc(out.len + 1);
    if (out.s == NULL) {
        out.len = 0;
        free(buf);
        return out;
        
    }
    
    memcpy(out.s, buf, out.len);
    out.s[out.len] = '\0';
    free(buf);
    
    return out;
    
};

SafeGetInstance SafeGet = {
    .ucread = ucread,
    .ucfgets = ucfgets
    
};