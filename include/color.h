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

    static float *setCurColor(int c, int &pre) {
        pre = curColor;
        curColor = c;
        if (c < 0 || c >= NR_COLORS)
            return getClearColor();
        return (float *) (rgbaColors[curColor]);
    }

    static float *setClearColor(int c = -1) {
        if (c == -1)
            c = clearColor;
        else if (c < 0 || c >= NR_COLORS)
            return NULL;
        clearColor = c;
        return (float *) (rgbaColors[c]);
    }
};


#endif //CG_COLOR_H
