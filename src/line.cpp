#include "line.h"
#include "window.h"

#include <cmath>

void LineAlgo::dda(int x1, int y1, int x2, int y2) {
    float k = (float) fabs((float) (y1 - y2) / (float) (x1 - x2));
    int delta = 1;
    bool delta_x = false;
    float x = x1, y = y1;
    if (k < 1) {
        delta_x = true;
        if (y1 > y2)
            k = -k;
        if (x1 > x2)
            delta = -1;
    } else {
        k = 1 / k;
        if (y1 > y2)
            delta = -1;
        if (x1 > x2)
            k = -k;
    }
    while (true) {
        win.write((int) x, (int) y);
        if (delta_x) {
            y += k;
            x += delta;
            if (delta < 0 && x <= x2) break;
            else if (delta > 0 && x >= x2) break;
        } else {
            x += k;
            y += delta;
            if (delta < 0 && y <= y2) break;
            else if (delta > 0 && y >= y2) break;
        }
    }
    win.write(x2, y2);
}

void LineAlgo::BresenhamLine(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int ux = ((dx > 0) << 1) - 1;
    int uy = ((dy > 0) << 1) - 1;
    int x = x1, y = y1, eps;

    eps = 0;
    dx = abs(dx);
    dy = abs(dy);
    if (dx > dy) {
        for (x = x1; x != x2 + ux; x += ux) {
            win.write(x, y);
            eps += dy;
            if ((eps << 1) >= dx) {
                y += uy;
                eps -= dx;
            }
        }
    } else {
        for (y = y1; y != y2 + uy; y += uy) {
            win.write(x, y);
            eps += dx;
            if ((eps << 1) >= dy) {
                x += ux;
                eps -= dy;
            }
        }
    }
}