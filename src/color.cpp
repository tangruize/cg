//
// Created by tangruize on 17-11-29.
//

#include "color.h"

const float Color::rgbaColors[][4] = {
        {1.0, 1.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {1.0, 1.0, 0.0, 0.0},
        {1.0, 0.0, 1.0, 0.0},
        {0.0, 1.0, 1.0, 0.0},
        {1.0, 0.5, 0.0, 0.0},
};

const char *const Color::strColors[] = {
        "White", "Black", "Red", "Green", "Blue", "Yellow", "Purple", "Cyan", "Orange"
};

int Color::curColor = C_BLACK;
int Color::clearColor = C_WHITE;

float *Color::setCurColor(int c, int &pre) {
    pre = curColor;
    curColor = c;
    if (c < 0 || c >= NR_COLORS)
        return getClearColor();
    return (float *) (rgbaColors[curColor]);
}

float *Color::setClearColor(int c) {
    if (c == -1)
        c = clearColor;
    else if (c < 0 || c >= NR_COLORS)
        return NULL;
    clearColor = c;
    return (float *) (rgbaColors[c]);
}
