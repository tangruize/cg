#include "window.h"

void Coordinate::coorTrans(float x, float y, float &fx, float &fy) {
    win.getHalfSize(fx, fy);
    fx = x / fx - 1;
    fy = 1 - y / fy;
}