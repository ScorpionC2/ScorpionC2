//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//
// Based on content in:
//  https://dev.to/ndesmic/linear-color-gradients-from-scratch-1a0e
//  https://gist.github.com/gskema/2f56dc2e087894ffc756c11e6de1b5ed
//  https://gist.github.com/Yyax13/f0efb154a72847ea93ab1a2e0698c53a
// 

#include "main.h"
#include "../../colors/main.h"

#include <stdio.h>

double calcFactor(int x, int y, int width, int height) {
    return (double)(x + y) / (double)(width + height - 2);
    
}

double interpolate(double start, double end, double factor) {
    return start + (end - start) * factor;
    
}

typedef struct {
    int     R;
    int     G;
    int     B;
    
} rgb_t;

typedef struct {
    int     R;
    int     G;
    int     B;
    
} rgb_td;

rgb_t parseRgb(char *color) {
    rgb_t out;
    sscanf(color, "%d;%d;%d", &out.R, &out.G, &out.B);
    
    return out;
    
};

void draw(int h, int w, char color1[15], char color2[15]) {
    rgb_t ccolor1 = parseRgb(color1);
    rgb_t ccolor2 = parseRgb(color2);
    
    double factor = calcFactor(0, 0, w, h);
    rgb_td color;
    
    color.R = interpolate(ccolor1.R, ccolor2.R, factor);    
    color.G = interpolate(ccolor1.G, ccolor2.G, factor);
    color.B = interpolate(ccolor1.B, ccolor2.B, factor);
    
    printf(_PREFIX _MOD_FG "%d;%d;%d" _END B_TL_CORNER RESET, color.R, color.G, color.B);
    
    for (int i = 0; i < w; i++) {
        factor = calcFactor(i, 0, w, h);
        color.R = interpolate(ccolor1.R, ccolor2.R, factor);    
        color.G = interpolate(ccolor1.G, ccolor2.G, factor);
        color.B = interpolate(ccolor1.B, ccolor2.B, factor);
        
        printf(_PREFIX _MOD_FG "%d;%d;%d" _END B_HORIZONTAL RESET, color.R, color.G, color.B);
        
    }
    
    factor = calcFactor(w - 1, 0, w, h);
    color.R = interpolate(ccolor1.R, ccolor2.R, factor);    
    color.G = interpolate(ccolor1.G, ccolor2.G, factor);
    color.B = interpolate(ccolor1.B, ccolor2.B, factor);
    
    printf(_PREFIX _MOD_FG "%d;%d;%d" _END B_TR_CORNER "\n" RESET, color.R, color.G, color.B);
    
    for (int i = 0; i < h; i++) {
        factor = calcFactor(0, i, w, h);
        color.R = interpolate(ccolor1.R, ccolor2.R, factor);    
        color.G = interpolate(ccolor1.G, ccolor2.G, factor);
        color.B = interpolate(ccolor1.B, ccolor2.B, factor);
        
        printf(_PREFIX _MOD_FG "%d;%d;%d" _END B_VERTICAL RESET, color.R, color.G, color.B);
        
        for (int ii = 0; ii < w; ii++) {
            printf(" ");
            
        }
        
        factor = calcFactor(w - 1, 0, w, h);
        color.R = interpolate(ccolor1.R, ccolor2.R, factor);    
        color.G = interpolate(ccolor1.G, ccolor2.G, factor);
        color.B = interpolate(ccolor1.B, ccolor2.B, factor);
        
        printf(_PREFIX _MOD_FG "%d;%d;%d" _END B_VERTICAL "\n" RESET, color.R, color.G, color.B);
        
    }
    
    factor = calcFactor(0, h - 1, w, h);
    color.R = interpolate(ccolor1.R, ccolor2.R, factor);    
    color.G = interpolate(ccolor1.G, ccolor2.G, factor);
    color.B = interpolate(ccolor1.B, ccolor2.B, factor);
    
    printf(_PREFIX _MOD_FG "%d;%d;%d" _END B_BL_CORNER RESET, color.R, color.G, color.B);
    
    for (int i = 0; i < w; i++) {
        factor = calcFactor(i, h - 1, w, h);
        color.R = interpolate(ccolor1.R, ccolor2.R, factor);    
        color.G = interpolate(ccolor1.G, ccolor2.G, factor);
        color.B = interpolate(ccolor1.B, ccolor2.B, factor);
        
        printf(_PREFIX _MOD_FG "%d;%d;%d" _END B_HORIZONTAL RESET, color.R, color.G, color.B);
        
    }
    
    factor = calcFactor(w - 1, h - 1, w, h);
    color.R = interpolate(ccolor1.R, ccolor2.R, factor);    
    color.G = interpolate(ccolor1.G, ccolor2.G, factor);
    color.B = interpolate(ccolor1.B, ccolor2.B, factor);
    
    printf(_PREFIX _MOD_FG "%d;%d;%d" _END B_BR_CORNER "\n" RESET, color.R, color.G, color.B);
    
};

const BoxxerInstance Box = {
    .draw = draw
    
};