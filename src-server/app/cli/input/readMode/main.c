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

#include "main.h"
#include <termios.h>

#define stdin 1

void turnEchoMode(bool_t _em) {
    struct termios cTerm;
    tcgetattr(stdin, &cTerm);
    cTerm.c_lflag &= _em ? ECHO : ~ECHO;
    tcsetattr(stdin, TCSAFLUSH, &cTerm);
    
};

void turnCanonicalMode(bool_t _cm) {
    struct termios cTerm;
    tcgetattr(stdin, &cTerm);
    cTerm.c_lflag &= _cm ? ICANON : ~ICANON;
    tcsetattr(stdin, TCSAFLUSH, &cTerm);
    
}

TerminalModeTogglerInstance TerminalModeToggler = {
    .turnCanonicalMode = turnCanonicalMode,
    .turnEchoMode = turnEchoMode
    
};