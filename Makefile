#
# Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
# Licensed under the MIT license. See LICENSE file in the project root for details.
#
# This file uses provided code at https://gist.github.com/prwhite/8168133?permalink_comment_id=4160255#gistcomment-4160255 for the help target
#

.PHONY: build debug clean help rebuild run test FORCE
.SILENT: build debug help rebuild run run-valgrind run-debug test

I_FLAGS 		:=  -I. \
								-Isrc \
								-Isrc/core \
								-Isrc/core/config \
								-Isrc/core/domain/encoders/services/xor \
								-Isrc/core/domain/encoders/types \
								-Isrc/core/shared/types \
								-Isrc/core/app/cli/colors \
								-Isrc/core/app/cli/loading \
								-Isrc/core/app/cli/logs \
								-Isrc/core/app/cli/ui/box \
								-Isrc/core/app/cli/input/readMode \
								-Isrc/core/app/cli/input/safeget \
								-Isrc/core/infra/fs \
								-Isrc/core/infra/hash \
								-Isrc/core/app/cli/input \
								-Isrc/core/shared/utils/math \
								-Isrc/core/shared/utils/mixers \
								-Isrc/core/shared/utils/comparision \
								-Isrc/core/infra/sched \
								-Isrc/core/infra/sched/context \
								-Isrc/core/infra/sched/processor \
								-Isrc/core/infra/sched/queue \
								-Isrc/core/infra/sched/tasks \
								-Isrc/core/infra/sched/tasks/stack \

LDFLAGS := 
LDLIBS := -lm

# e.g.: make debug OPT_FLAGS="-DFLAG_DEBUG" or anyother gcc setting
OPT_FLAGS :=

# Optimize and debug may not work together
OPTIMIZE_FLAGS 	:= 	-O3 -march=native -flto
DEBUG_FLAGS 	:= 	-Og -dA -dD -ggdb3 -Wall -Wextra -Wformat=2 -Wshadow -Wundef \
									-fsanitize=address -fno-omit-frame-pointer -no-pie -fno-inline

CC				:= 	gcc
CC_FLAGS		:=	$(I_FLAGS) $(OPTIMIZE_FLAGS)

TARGET_DIR		:=	build/
TARGET			:=	$(TARGET_DIR)scorpionc2-server
CORE_SRC :=		\
							src/core/app/cli/logs/main.c \
							src/core/shared/utils/random/main.c \
							src/core/infra/hash/main.c \
							src/core/domain/encoders/services/main.c \
							src/core/domain/encoders/services/xor/main.c \
							src/core/app/cli/ui/box/main.c \
							src/core/app/cli/input/readMode/main.c \
							src/core/app/cli/input/safeget/main.c \
							src/core/app/cli/input/main.c \
							src/core/infra/fs/main.c \
							src/core/shared/utils/mixers/main.c \
							src/core/infra/sched/main.c \
							src/core/infra/sched/tasks/main.c \
							src/core/infra/sched/tasks/stack/main.c \
							src/core/infra/sched/processor/main.c \
							src/core/infra/sched/queue/main.c \

SRC_ENTRYPOINT	:=	src/core/app/main.c $(CORE_SRC)

AS := nasm
AS_FLAGS := -f elf64

AS_SRC := \
	src/core/infra/sched/context/main.s \
	src/core/infra/sched/tasks/main.s

AS_OBJ_DIR := build/asm
AS_OBJS := $(patsubst %.s,$(AS_OBJ_DIR)/%.o,$(AS_SRC))
AS_OBJ := build/asm.o

$(AS_OBJ_DIR)/%.o: %.s FORCE
	@mkdir -p $(dir $@)
	@$(AS) $(AS_FLAGS) $< -o $@

all: help

FORCE:

build: ## Build the project to build/scorpionc2-server
	mkdir -p $(TARGET_DIR) $(AS_OBJ_DIR)
	@$(MAKE) --no-print-directory $(AS_OBJS)
	ld -r $(AS_OBJS) -o $(AS_OBJ)
	
	$(CC) \
		$(CC_FLAGS) \
		$(OPT_FLAGS) \
		-o $(TARGET) \
		$(SRC_ENTRYPOINT) \
		$(AS_OBJ) \
		$(LDLIBS)
	
	rm -rf $(AS_OBJ)

DEBUG_TARGET_DIR := build/
DEBUG_TARGET := $(DEBUG_TARGET_DIR)scorpionc2-server-debug

debug: ## Build the project to build/scorpionc2-server-debug with debug flags
	mkdir -p $(DEBUG_TARGET_DIR) $(AS_OBJ_DIR)
	@$(MAKE) --no-print-directory $(AS_OBJS)
	ld -r $(AS_OBJS) -o $(AS_OBJ)
	
	$(CC) \
		$(I_FLAGS) $(DEBUG_FLAGS) \
		$(OPT_FLAGS) \
		-o $(DEBUG_TARGET) \
		$(SRC_ENTRYPOINT) \
		$(AS_OBJ) \
		$(LDLIBS)
	
	rm -rf $(AS_OBJ)


clean: ## Remove build binary
	rm -rf $(TARGET)

help: ## Show this menu
	@grep -E '^[a-z.A-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

rebuild: ## Remove old build and build again
	rm -rf $(TARGET)
	mkdir -p $(TARGET_DIR) $(AS_OBJ_DIR)
	@$(MAKE) --no-print-directory $(AS_OBJS)
	ld -r $(AS_OBJS) -o $(AS_OBJ)
	
	$(CC) \
		$(CC_FLAGS) \
		$(OPT_FLAGS) \
		-o $(TARGET) \
		$(SRC_ENTRYPOINT) \
		$(AS_OBJ) \
		$(LDLIBS)
	
	rm -rf $(AS_OBJ)

RUN_TARGET_DIR := /tmp/scorpion-runs/$(shell date --iso=seconds)
RUN_TARGET := $(RUN_TARGET_DIR)/scorpionc2
	
run: ## Run the project in a tmp file
	rm -rf $(RUN_TARGET) $(RUN_TARGET_DIR)
	mkdir -p $(RUN_TARGET_DIR) $(AS_OBJ_DIR)
	@$(MAKE) --no-print-directory $(AS_OBJS)
	ld -r $(AS_OBJS) -o $(AS_OBJ)
	
	$(CC) \
		$(CC_FLAGS) \
		$(OPT_FLAGS) \
		-o $(RUN_TARGET) \
		$(SRC_ENTRYPOINT) \
		$(AS_OBJ) \
		$(LDLIBS)
	
	rm -rf $(AS_OBJ)
	$(RUN_TARGET)
	rm -rf $(RUN_TARGET) $(RUN_TARGET_DIR)

VALGRIND_PATH := "/usr/bin/valgrind"
VALGRIND_FLAGS := 	--leak-check=summary \
					--errors-for-leak-kinds=definite \
					-s \
					$(RUN_TARGET)
					
VALGRIND_TARGET := $(VALGRIND_PATH) $(VALGRIND_FLAGS)
DEBUG_CC_FLAGS	:= $(I_FLAGS) $(DEBUG_FLAGS)

run-valgrind: ## Run the project in a tmp file and check it with valgrind
	rm -rf $(RUN_TARGET) $(RUN_TARGET_DIR)
	mkdir -p $(RUN_TARGET_DIR) $(AS_OBJ_DIR)
	@$(MAKE) --no-print-directory $(AS_OBJS)
	ld -r $(AS_OBJS) -o $(AS_OBJ)
	
	$(CC) \
		$(CC_FLAGS) \
		$(OPT_FLAGS) \
		-o $(RUN_TARGET) \
		$(SRC_ENTRYPOINT) \
		$(AS_OBJ) \
		$(LDLIBS)
	
	rm -rf $(AS_OBJ)
	$(VALGRIND_TARGET)
	rm -rf $(RUN_TARGET) $(RUN_TARGET_DIR)

run-debug: ## Run the project in a tmp file with debug flags
	rm -rf $(RUN_TARGET) $(RUN_TARGET_DIR)
	mkdir -p $(RUN_TARGET_DIR) $(AS_OBJ_DIR)
	@$(MAKE) --no-print-directory $(AS_OBJS)
	ld -r $(AS_OBJS) -o $(AS_OBJ)
	
	$(CC) \
		$(DEBUG_CC_FLAGS) \
		$(OPT_FLAGS) \
		-o $(RUN_TARGET) \
		$(SRC_ENTRYPOINT) \
		$(AS_OBJ) \
		$(LDLIBS)
	
	rm -rf $(AS_OBJ)
	$(RUN_TARGET)
	rm -rf $(RUN_TARGET) $(RUN_TARGET_DIR)

TEST_TARGET_DIR := /tmp/scorpion-tests/$(shell date --iso=seconds)
TEST_TARGET := $(TEST_TARGET_DIR)/ScorpionC2-tests
TEST_SRC := $(CORE_SRC) \
						src/tests/engine/main.c \
						src/tests/engine/unit/main.c \
						src/tests/unit/encoders/xor.c \
						src/tests/unit/hash/djb2.c

TEST_I_FLAGS := $(I_FLAGS) \
								-Isrc/tests/engine \
								-Isrc/tests/engine/unit \
								-Isrc/tests/unit/encoders \
								-Isrc/tests/unit/hash

test: ## Run the project's tests
	rm -rf $(TEST_TARGET) $(TEST_TARGET_DIR)
	mkdir -p $(TEST_TARGET_DIR) $(AS_OBJ_DIR)
	@$(MAKE) --no-print-directory $(AS_OBJS)
	ld -r $(AS_OBJS) -o $(AS_OBJ)
	
	$(CC) \
		-fsanitize=address \
		$(OPT_FLAGS) \
		-g $(TEST_I_FLAGS) \
		-o $(TEST_TARGET) \
		$(TEST_SRC) \
		$(AS_OBJ) \
		$(LDLIBS)
	
	$(TEST_TARGET)
	rm -rf $(TEST_TARGET) $(TEST_TARGET_DIR)
