#
# Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
# Licensed under the MIT license. See LICENSE file in the project root for details.
# 
# This file uses provided code at https://gist.github.com/prwhite/8168133?permalink_comment_id=4160255#gistcomment-4160255 for the help target
#

.PHONY: build clean help rebuild

I_FLAGS 		:=  -Isrc-server/domain/encoders/services/xor \
					-Isrc-server/domain/encoders/types \
					-Isrc-server/shared/types \
					-Isrc-server/app/cli/colors \
					-Isrc-server/app/cli/loading
			
OPTIMIZE_FLAGS 	:= 	-0s
DEBUG_FLAGS 	:= 	-0g -dA -dD -ggdb

USE_DEBUG		:= 	false
USE_OPTIMIZE	:=	true

# Optimize and debug may not work together

CC				:= 	gcc
CC_FLAGS		:=	$(I_FLAGS)
ifeq ($(USE_DEBUG),true)
	CC_FLAGS 	+=	$(DEBUG_FLAGS)
endif

ifeq ($(USE_OPTIMIZE),true)
	CC_FLAGS	+=	$(OPTIMIZE_FLAGS)
endif

TARGET			:=	build/scorpionc2-server
SRC_ENTRYPOINT	:=	src-server/app/main.c

all: help

build: ## Build the project to build/scorpionc2-server
	$(CC) $(CC_FLAGS) -o $(TARGET) $(SRC_ENTRYPOINT)

clean: ## Remove build binary
	rm -rf $(TARGET)

help: ## Show this menu
	@grep -E '^[a-z.A-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

rebuild:
	make clean
	make build