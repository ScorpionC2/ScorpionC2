//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "main.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int appendFile(string_t path, const bytes_t *src) {
    if (!path.s || !path.len)
        return 1;

    FILE *file = fopen(path.s, "ab+");
    if (file == NULL)
        return -1;

    fwrite(src->b, sizeof(src->b[0]), src->len, file);
    fclose(file);

    return 0;
};

int countLines(string_t path) {
    int count = 0;
    if (path.s == NULL || path.len <= 0)
        return -1;

    FILE *file = fopen(path.s, "r");
    if (file == NULL)
        return -1;

    for (int c = fgetc(file); c != EOF; c = fgetc(file)) {
        if (c == '\n')
            count++;
    }

    fclose(file);
    return count;
}

int getLine(string_t path, int line, bytes_t *out) {
    if (path.s == NULL || path.len == 0)
        return 1;

    int lines = countLines(path);
    if (lines < 0)
        return 1;

    if (line > lines)
        return -1;

    FILE *file = fopen(path.s, "r");
    if (file == NULL)
        return 1;

    int count = 1;
    char lineBuf[1024];
    bool_t found = FALSE;

    while (fgets(lineBuf, sizeof(lineBuf), file) != NULL) {
        if (count == line) {
            found = TRUE;
            break;
        }

        count++;
    }

    fclose(file);

    if (!found) {
        out->len = 0;
        out->b = NULL;
        return -1;
    }

    char *p = memchr(lineBuf, '\n', sizeof(lineBuf));
    if (p != NULL) {
        *p = '\0';
    }

    out->len = strlen(lineBuf) + 1;
    out->b = malloc(out->len);
    memcpy(out->b, lineBuf, out->len);

    return 0;
};

int makeDir(string_t path, mode_t perm) {
    if (mkdir(path.s, perm) == -1) {
        return 1;
    } else {
        return 0;
    }
}

int rmDir(string_t path) {
    if (rmdir(path.s) == -1) {
        return 1;
    } else {
        return 0;
    }
}

int readFIle(string_t path, bytes_t *out) {
    if (!out) {
        return 1;
    }

    out->b = NULL;
    FILE *file = fopen(path.s, "rb");
    if (file == NULL)
        return -1;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);

    if (size == -1) {
        fclose(file);
        return 1;
    }

    rewind(file);
    if (size == 0) {
        out->b = NULL;
        fclose(file);
        return 0;
    }

    out->b = malloc(size);
    out->len = size;

    if (out->b == NULL) {
        fclose(file);
        return -1;
    }

    size_t read = fread(out->b, 1, size, file);
    fclose(file);

    if (read != size) {
        free(out->b);
        out->b = NULL;
        return -1;
    }

    return 0;
}

int writeFile(string_t path, const bytes_t *src) {
    FILE *fp = fopen(path.s, "wb");

    if (!fp) {
        return 1;
    }

    size_t writed = fwrite(src->b, 1, src->len, fp);

    if (writed !=
        src->len) { // If writed is not the same size as src->len it will return -1
        return -1;
    }

    fclose(fp);
    return 0;
}

FsInstance Files = {.countLines = countLines,
                    .appendFile = appendFile,
                    .getLine = getLine,
                    .makeDir = makeDir,
                    .rmDir = rmDir,
                    .readFIle = readFIle,
                    .writeFile = writeFile};
