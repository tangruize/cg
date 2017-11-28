#ifndef CG_SHAPE_H
#define CG_SHAPE_H

enum SHAPES {
    SPOINT, THREAD, TRIANGLE, RECTANGLE, CIRCLE
};

enum COLORS {
    BLACK, RED, GREEN, BLUE
};

enum MENUS {
    LOAD, SAVE, CLEAR
};

struct point {
    int x, y;
    int color;
};

struct thread {
    int x1, y1;
    int x2, y2;
    int color;
};

struct triangle {
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int color;
};

struct rectangle {
    int x1, y1;
    int x2, y2;
    int color;
};

struct circle {
    int x, y;
    float r;
    int color;
};

extern const char *shapes[5];
extern const char *colors[4];
extern int shape;
extern int color;

#endif