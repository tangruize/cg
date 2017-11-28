#include "window.h"
#include "coordinate.h"

const float HALF_WIDTH = (float) WIDTH / 2;
const float HALF_HEIGHT = (float) HEIGHT / 2;

void coorTrans(float x, float y, float *fx, float *fy) {
    *fx = x / HALF_WIDTH - 1;
    *fy = 1 - y / HALF_HEIGHT;
}