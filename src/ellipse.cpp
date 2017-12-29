//
// Created by tangruize on 17-11-29.
//

#include "ellipse.h"
#include "window.h"

#include <cmath>

int Oval::tmp_x, Oval::tmp_y;

void Oval::doDraw(float x, float y, bool isCicle) {
    win.write((int) (x + tmp_x), (int) (y + tmp_y));
    win.write((int) (-x + tmp_x), (int) (-y + tmp_y));
    win.write((int) (x + tmp_x), (int) (-y + tmp_y));
    win.write((int) (-x + tmp_x), (int) (y + tmp_y));
    if (isCicle) {
        win.write((int) (y + tmp_x), (int) (-x + tmp_y));
        win.write((int) (-y + tmp_x), (int) (x + tmp_y));
        win.write((int) (y + tmp_x), (int) (x + tmp_y));
        win.write((int) (-y + tmp_x), (int) (-x + tmp_y));
    }
}

void CircleAlgo::BresenhamCircle(int xc, int yc, float r) {
    float x = 0, y = r, p = 3 - 2 * r;
    tmp_x = xc;
    tmp_y = yc;
    for (; x <= y; ++x) {
        doDraw(x, y, true);
        if (p >= 0) {
            p += 4 * (x - y) + 10;
            --y;
        } else {
            p += 4 * x + 6;
        }
    }
}

//wrong implement!
//void EllipseAlgo::BresenhamEllipse(int mx, int my, int a, int b) {
//    int squareA = a * a, squareB = b * b;
//    int x = 0, y = b;
//    int d = squareB * 2 - squareA * (2 * b - 1);
//    int px = (int) ((double) squareA / sqrt(squareA + squareB));
//
//    tmp_x = mx;
//    tmp_y = my;
//
//    doDraw(x, y);
//    while (x <= px) {
//        if (d < 0)
//            d += 2 * squareB * (2 * x + 3);
//        else
//            d += 2 * squareB * (2 * x + 3) - 4 * squareA * (y-- - 1);
//        doDraw(x++, y);
//    }
//
//    d = squareB * (x * x + x) + squareA * (y * y - y) - squareA * squareB;
//    while (y >= 0) {
//        doDraw(x, y--);
//        if (d < 0)
//            d = d - 2 * squareA * y - squareA + 2 * squareB * ++x + 2 * squareB;
//        else
//            d = d - 2 * squareA * y - squareA;
//    }
//}

void EllipseAlgo::BresenhamEllipse(int cx, int cy, int a, int b) {
    // use the x radius.
    int x = a, y = 0;
    int sqrB = b * b, sqrA = a * a;
    int xChange = sqrB * (1 - 2 * a), yChange = sqrA, err = 0;
    int aDoubleSqr = 2 * sqrA, bDoubleSqr = 2 * sqrB;
    int stopX = bDoubleSqr * a, stopY = 0;

    tmp_x = cx;
    tmp_y = cy;

    while (stopX >= stopY) {
        doDraw(x, y++);
        stopY += aDoubleSqr;
        err += yChange;
        yChange += aDoubleSqr;
        if ((2 * err + xChange) > 0) {
            x--;
            stopX -= bDoubleSqr;
            err += xChange;
            xChange += bDoubleSqr;
        }
    }

    y = b;
    xChange = sqrB;
    yChange = sqrA * (1 - (2 * b));
    x = err = stopX = 0;
    stopY = aDoubleSqr * b;
    while (stopX <= stopY) {
        doDraw(x++, y);
        stopX += bDoubleSqr;
        err += xChange;
        xChange += bDoubleSqr;
        if ((2 * err + yChange) > 0) {
            y--;
            stopY -= aDoubleSqr;
            err += yChange;
            yChange += aDoubleSqr;
        }
    }
}