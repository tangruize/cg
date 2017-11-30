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
