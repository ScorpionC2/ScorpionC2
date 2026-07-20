//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
// Implementations based in:
//  https://stackoverflow.com/questions/6698161/getting-raw-input-from-console-using-c-or-c
//  https://man7.org/linux/man-pages/man1/stty.1.html
//  https://medium.com/@krithikanithyanandam/the-elegant-architecture-of-ptys-behind-your-terminal-a-quick-byte-b724a50a98b4
//  https://www.reddit.com/r/C_Programming/comments/1ipbyp4/canonical_vs_non_canonical_mode_of_inputs/
//  https://stackoverflow.com/questions/59922972/how-to-stop-echo-in-terminal-using-c
//  https://pubs.opengroup.org/onlinepubs/009696799/functions/tcsetattr.html
//

#include "src-server/shared/types/main.h"
#include "src-server/app/cli/input/readMode/main.h"
#include <termios.h>
#include <unistd.h>

static bool_t echoModeOn = FALSE;
static bool_t canonicalModeOn = FALSE;

void turnMode(uint_t mode) {
    struct termios cTerm;
    tcgetattr(STDIN_FILENO, &cTerm);

    if (mode & ECHO_MODE) {
        echoModeOn = !echoModeOn;
        cTerm.c_lflag ^= ECHO;
    }

    if (mode & CANONICAL_MODE) {
        canonicalModeOn = !canonicalModeOn;
        cTerm.c_lflag ^= ICANON;
    }

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &cTerm);
};

TerminalModeChangerInstance TerminalModeToggler = {.turnMode = turnMode};
