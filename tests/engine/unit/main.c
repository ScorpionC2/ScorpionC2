#include "tests/engine/unit/main.h"
#include "src-server/app/cli/colors/main.h"
#include "src-server/app/cli/logs/main.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void registerUnitTest(const struct UnitTest *test) {
    // Prevent writing to null ptr
    if (unitTests == NULL) {
        unitTests =
            (struct UnitTest *)malloc(unitTestsSize * sizeof(struct UnitTest));
        if (unitTests == NULL) {
            Logger.errorf(RESET FG_WHITE_ITALIC "Can't malloc:" RESET
                                                " " FG_YELLOW_ITALIC "%s" RESET
                                                "\n",
                          strerror(errno));
            return;
        }
    }

    // Prevent heap BoF
    if (unitTestsIndex >= unitTestsSize - 1) {
        unitTestsSize <<= 1; // unitTestsSize *= 2
        unitTests = realloc(unitTests, unitTestsSize * sizeof(struct UnitTest));
        if (unitTests == NULL) {
            Logger.errorf(RESET FG_WHITE_ITALIC "Can't realloc:" RESET
                                                " " FG_YELLOW_ITALIC "%s" RESET
                                                "\n",
                          strerror(errno));
            return;
        }
    }

    // Prevent using a null new test
    if (test == NULL) {
        Logger.errorln(FG_WHITE_ITALIC "Can't create a null unit test:" RESET
                                       " " FG_YELLOW_ITALIC
                                       "Received NULL pointer as test" RESET);
        return;
    }

    unitTests[unitTestsIndex++] = *test;
    return;
};

void runUnitTests(const uint32_t *ac) {
    if (unitTestsSize <= 0) {
        Logger.errorf(RESET FG_WHITE_ITALIC
                      "Can't run a invalid quantity of unit tests:" RESET
                      " " FG_YELLOW_ITALIC "Attempt to "
                      "run %u tests" RESET "\n",
                      unitTestsSize);
        return;
    }

    if (unitTests == NULL) {
        Logger.errorf(RESET FG_WHITE_ITALIC
                      "Can't run unit tests:" RESET " " FG_YELLOW_ITALIC
                      "Attempt to run unit tests by a NULL pointer" RESET "\n");
        return;
    }

    for (size_t i = 0; i < unitTestsIndex; i++) {
        struct UnitTest curTest = unitTests[i];
        if (curTest.func == NULL) {
            ac++;
            Logger.warnf(FG_WHITE_ITALIC "Test" RESET " " FG_CYAN_ITALIC
                                         "%s" RESET " " FG_WHITE_ITALIC
                                         "failed:" RESET " " FG_YELLOW_ITALIC
                                         "Can't run a NULL function" RESET "\n",
                         curTest.name.s);
            continue;
        }

        bool_t result = curTest.func();
        if (result == FALSE) {
            ac++;
            Logger.warnf(FG_WHITE_ITALIC "Test" RESET " " FG_CYAN_ITALIC
                                         "%s" RESET " " FG_WHITE_ITALIC
                                         "failed:" RESET " " FG_YELLOW_ITALIC
                                         "The test returned FALSE" RESET "\n",
                         curTest.name.s);
            continue;
        }

        Logger.infof(FG_GREEN_ITALIC "Successfuly" FG_WHITE_ITALIC
                                     " run test" RESET " " FG_CYAN_ITALIC
                                     "%s" RESET "\n",
                     curTest.name.s);
    }
}

size_t unitTestsIndex = 0;
size_t unitTestsSize = 2048;
struct UnitTest *unitTests = NULL;
