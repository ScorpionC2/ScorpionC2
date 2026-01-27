//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "../types/main.h"

/*
 * This file must contain all encoders as pointers to their implementations.
 * Example:
 *      // main.h
 *      
 *      extern EncoderInstance *Xor;
 * 
 *      
 *      // main.c
 *      
 *      #include "xor/main.h" // this main.h declares extern XorInstance.
 *      
 *      const EncoderInstance *Xor = &XorInstance;
 *      
 * The idea is to have all encoders in just one file, without the need to include every implementation.
 */
 
 extern EncoderInstance *Xor;