//
// Created by tangruize on 17-11-29.
//

#ifndef CG_COLOR_H
#define CG_COLOR_H

#include <cstdlib>

class Color {
public:
    static const int NR_COLORS = 9;
    static const char *const strColors[NR_COLORS];
    enum COLORS {
        C_DESTROY = -1, C_WHITE = 0, C_BLACK, C_RED, C_GREEN, C_BLUE, C_YELLOW, C_PURPLE, C_CYAN, C_ORANGE
    };
    static const float rgbaColors[NR_COLORS][4];

private:
    static int curColor;
    static int clearColor;

public:
    static int getCurColor() { return curColor; }

    static float *getClearColor() { return (float *) (rgbaColors[clearColor]); }

    static int getClearColorIndex() { return clearColor; }

    static float *setCurColor(int c, int &pre);

    static float *setClearColor(int c = -1);
};


#endif //CG_COLOR_H
