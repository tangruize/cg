#include "window.h"
#include <cmath>
#include <iostream>

using namespace std;

void Coordinate::coorTrans(float x, float y, float &fx, float &fy) {
    win.getHalfSize(fx, fy);
    fx = x / fx - 1;
    fy = 1 - y / fy;
}

const char *const Coordinate::strTrans[] = {
        "Move", "Rotate", "Zoom"
};

void Coordinate::setSrc(int x, int y) {
    srcX = x;
    srcY = y;
}

void Coordinate::setSrc2(int x, int y) {
    srcX2 = x;
    srcY2 = y;
}

void Coordinate::setDest(int x, int y) {
    toX = x;
    toY = y;
    switch (type) {
        case T_MOVE:
            srcX2 = x, srcY2 = y;
            return;
        case T_ZOOM: {
            if (srcX2 == srcX)
                info = 1;
            else
                info = (double) (toX - srcX) / (srcX2 - srcX);
            if (srcY2 == srcY)
                info2 = 1;
            else
                info2 = (double) (toY - srcY) / (srcY2 - srcY);
//            info = fabs(info);
//            info2 = fabs(info2);
            return;
        }
        case T_ROTATE: {
            int a[2] = {srcX2 - srcX, srcY2 - srcY}, b[2] = {toX - srcX, toY - srcY};
            double ab, a1, b1, cosr;
            ab = a[0] * b[0] + a[1] * b[1];
            a1 = sqrt((double) a[0] * a[0] + (double) a[1] * a[1]);
            b1 = sqrt((double) b[0] * b[0] + (double) b[1] * b[1]);
            cosr = ab / a1 / b1;
            info = acos(cosr);
            if ((srcX - x) * (srcY2 - y) - (srcY - y) * (srcX2 - x) > 0)
                info = -info;
            return;
        }
        default:
            return;
    }
}

void Coordinate::setType(int t) {
    type = t;
}

void Coordinate::getTransResult(int &x, int &y) {
    switch (type) {
        case T_MOVE:
            x += srcX2 - srcX;
            y += srcY2 - srcY;
            break;
        case T_ZOOM:
            x = (int) (x * info + srcX * (1 - info));
            y = (int) (y * info2 + srcY * (1 - info2));
            break;
        case T_ROTATE: {
            double cosTheta = cos(info), sinTheta = sin(info);
            int saved = x;
            x = (int) (srcX + (x - srcX) * cosTheta + (y - srcY) * sinTheta);
            y = (int) (srcY - (saved - srcX) * sinTheta + (y - srcY) * cosTheta);
            break;
        }
        default:
            break;
    }
}

int Coordinate::srcX, Coordinate::srcY;
int Coordinate::srcX2, Coordinate::srcY2;
int Coordinate::toX, Coordinate::toY;
double Coordinate::info, Coordinate::info2;
int Coordinate::type;