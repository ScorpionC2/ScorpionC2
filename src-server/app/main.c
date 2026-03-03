//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "../shared/types/main.h"
#include "../shared/utils/random/main.h"

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "commands/main.h"
#include "cli/logs/main.h"
#include "cli/input/main.h"

void init() {
    Random.seed(time(NULL));
    Random.seed(Random.rand());

}

int main() {
    init();

    InputSettings conf;
    conf.prompt.s = "scorpion> ";
    conf.prompt.len = strlen(conf.prompt.s);
    conf.histPath.s = "./.scorpion_history";
    conf.histPath.len = strlen(conf.histPath.s);
    conf.histLimit = 1000;

    while (1) {
        string_t input;
        Input.readline(conf, &input);
        if (input.s == NULL) {
            continue;
        }

        int res = Commands.execute(input);
        free(input.s);
        if (res == 1) {
            Logger.infoln("Exiting...");
            break;
        } else if (res == -1) {
            Logger.warnln("Command execution error");
            continue;
        }
    }
    return 0;
}
