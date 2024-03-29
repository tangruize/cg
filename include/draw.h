#ifndef CG_DRAW_H
#define CG_DRAW_H

#include <vector>
#include <utility>

class Draw {
public:
    static void point(int x, int y);

    static void line(int x1, int y1, int x2, int y2);

    static void circle(int x, int y, float r);

    static void ellipse(int x, int y, int a, int b);

    static void fill(int x, int y);

    static void setFillVertice(std::vector<int> &xd, std::vector<int> &yd);

    static void curve(std::vector<std::pair<int , int>> &vertice);

};
#endif