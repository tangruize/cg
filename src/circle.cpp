#include "circle.h"
#include "coordinate.h"

#include <GL/glut.h>

static int tmp_x, tmp_y;

static void doDrawCircle(float x, float y) {
    float fx, fy;
    coorTrans(x + tmp_x, y + tmp_y, &fx, &fy);
    glVertex2f(fx, fy);
    coorTrans(x + tmp_x, -y + tmp_y, &fx, &fy);
    glVertex2f(fx, fy);
    coorTrans(-x + tmp_x, y + tmp_y, &fx, &fy);
    glVertex2f(fx, fy);
    coorTrans(-x + tmp_x, -y + tmp_y, &fx, &fy);
    glVertex2f(fx, fy);
    coorTrans(y + tmp_x, x + tmp_y, &fx, &fy);
    glVertex2f(fx, fy);
    coorTrans(y + tmp_x, -x + tmp_y, &fx, &fy);
    glVertex2f(fx, fy);
    coorTrans(-y + tmp_x, x + tmp_y, &fx, &fy);
    glVertex2f(fx, fy);
    coorTrans(-y + tmp_x, -x + tmp_y, &fx, &fy);
    glVertex2f(fx, fy);
}

void BresenhamCircle(int xc, int yc, float r) {
    float x = 0, y = r, p = 3 - 2 * r;
    tmp_x = xc;
    tmp_y = yc;
    glBegin(GL_POINTS);
    for (; x <= y; ++x) {
        doDrawCircle(x, y);
        if (p >= 0) {
            p += 4 * (x - y) + 10;
            --y;
        } else {
            p += 4 * x + 6;
        }
    }
    glEnd();
}