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
#include "src-server/app/cli/input/readMode/main.h"
#include "src-server/app/cli/logs/main.h"
#include "src-server/infra/fs/main.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TOGGLE_TERMINAL_MODE                                                   \
    TerminalModeToggler.turnMode(ECHO_MODE | CANONICAL_MODE);

ssize_t _parseStdin(char *ch) { return read(STDIN_FILENO, ch, 1); }

bool_t _checkBuf(char **buf, size_t *bufLen, size_t *bufCap) {
    if ((*bufLen + 1) >= *bufCap) {
        while ((*bufLen + 1) >= *bufCap) {
            if (*bufCap > SIZE_MAX / 2)
                return FALSE;

            *bufCap <<= 1; // Times two optimized
        }

        // Here i can directly assign the new reallocated buffer to buf, but if i do that
        // and the realloc fails, the caller won't know if the buf should be freed or not
        char *newBuf = realloc(*buf, *bufCap);
        if (newBuf == NULL) {
            return FALSE;
        }

        *buf = newBuf;
    }

    return TRUE;
}

/*
 * This function should only be called after getting the pressed arrow
 */
void _parseHist(char ch, int *curHistL, int histFileLines, string_t histPath,
                char **outLine) {
    if (histFileLines == 0)
        return;

    int histLineProcessed = 0;
    switch (ch) {
        case 0x41: { // Arrow UP, must get the previous line
            histLineProcessed =
                ((*curHistL)-- - 1 + histFileLines) % histFileLines;

            break;
        }

        case 0x42: { // Arrow Down
            histLineProcessed =
                ((*curHistL)++ + 1 + histFileLines) % histFileLines;

            break;
        }

        default: {
            return;
        }
    }

    bytes_t l;
    if (Files.getLine(histPath, histLineProcessed, &l) != 0)
        return;

    char *tmp = malloc(l.len);
    if (tmp == NULL)
        return;

    memcpy(tmp, l.b, l.len);
    free(l.b);

    free(*outLine);
    *outLine = tmp;

    return;
}

/*
 * This function should only be called if the two previous character is 0x1B and 0x5B
 *
 * Return values:
 *   0: If can continue
 *  -1: Error
 */
int _handleArrows(int *curHistL, int histFileLines, string_t histPath,
                  size_t *bufLen, char **buf, size_t *bufCap) {
    char ch = 0x00;
    if (_parseStdin(&ch) <= 0) {
        return -1;
    }

    char *histL = NULL;
    _parseHist(ch, curHistL, histFileLines, histPath, &histL);

    if (histL == NULL)
        return 0;

    *bufLen = strlen(histL);
    if (_checkBuf(buf, bufLen, bufCap) == FALSE) {
        return -1;
    }

    memcpy(*buf, histL, *bufLen);
    (*buf)[*bufLen] = '\0';
    free(histL);

    return 0;
};

void _refreshStdout(int promptSize, char *buf) {
    printf("\x1b[1G\x1b[%dC%s\x1b[K", promptSize, buf);
    fflush(stdout);
}

/*
 * Return values:
 *   0: If can continue
 *   1: End of buffer (like newLine)
 *  -1: Error (free buf, toggle terminal mode and return NULL)
 */
int _handleCharacter(char ch, int *curHistL, int histFileLines,
                     string_t histPath, size_t *bufLen, size_t *bufCap,
                     char **buf, int promptSize) {
    switch (ch) {
        case 0x0A: { // New line
            putchar(0x0A);
            return 1;
        }

        case 0x1B: { // Start of arrow payload
            char ch2 = 0x00;
            if (_parseStdin(&ch2) <= 0)
                return -1;

            if (ch2 == 0x5B) {
                if (_handleArrows(curHistL, histFileLines, histPath, bufLen,
                                  buf, bufCap) < 0)
                    return -1;

                _refreshStdout(promptSize, *buf);
                return 0;
            }

            break;
        }

        case 0x7F:
        case 0x08: {
            if (*bufLen > 0) {
                (*bufLen)--;
                (*buf)[*bufLen] = '\0';
            }

            _refreshStdout(promptSize, *buf);

            break;
        }

        default: {
            (*buf)[(*bufLen)++] = ch;
            (*buf)[*bufLen] = '\0';

            putc(ch, stdout);
            fflush(stdout);

            break;
        }
    }

    return 0;
};

// ucfgets - Ultra-incredible-handmade galactic empire engineer's Custom FGETS
char *ucfgets(string_t histPath, int promptSize) {
    TOGGLE_TERMINAL_MODE

    size_t bufCap = 65536;
    size_t bufLen = 0;
    char *buf = malloc(bufCap);
    if (buf == NULL) {
        TOGGLE_TERMINAL_MODE;
        return NULL;
    }

    char ch = '\0';

    int currentHistLine;
    int histFileLines = Files.countLines(histPath);
    if (histFileLines < 0) {
        free(buf);
        TOGGLE_TERMINAL_MODE
        return NULL;
    }

    currentHistLine = histFileLines;

    while (TRUE) {
        if (_parseStdin(&ch) <= 0) {
            TOGGLE_TERMINAL_MODE;
            free(buf);
            return NULL;
        }

        if (_checkBuf(&buf, &bufLen, &bufCap) == FALSE) {
            free(buf);
            TOGGLE_TERMINAL_MODE
            return NULL;
        }

        switch (_handleCharacter(ch, &currentHistLine, histFileLines, histPath,
                                 &bufLen, &bufCap, &buf, promptSize)) {
            case 0: {
                continue;
            }

            case 1: {
                TOGGLE_TERMINAL_MODE
                return buf;
            }

            case -1: {
                TOGGLE_TERMINAL_MODE
                free(buf);
                return NULL;
            }
        }
    }
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

SafeGetInstance SafeGet = {.ucread = ucread, .ucfgets = ucfgets

};
