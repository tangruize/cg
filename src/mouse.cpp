#include "shape.h"
#include "draw.h"
#include "mouse.h"

#include <GL/glut.h>
#include <math.h>

static struct point tmp_point;
static struct thread tmp_thread;
static struct triangle tmp_triangle;
static struct rectangle tmp_rectangle;
static struct circle tmp_circle;

static int cnt_thread = 0;
static int cnt_triangle = 0;
static int cnt_rectangle = 0;
static int cnt_circle = 0;

void resetCnt() {
    cnt_thread = 0;
    cnt_triangle = 0;
    cnt_rectangle = 0;
    cnt_circle = 0;
}

static void casePoint(int x, int y) {
    tmp_point.color = color;
    tmp_point.x = x;
    tmp_point.y = y;
    drawPoint(tmp_point);
}

void mouseClick(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON)
        return;
    if (state != GLUT_DOWN)
        return;
    switch (shape) {
        case SPOINT:
            casePoint(x, y);
            addPoint(tmp_point);
            break;
        case THREAD:
            tmp_thread.color = color;
            if (cnt_thread % 2 == 0) {
                tmp_thread.x1 = x;
                tmp_thread.y1 = y;
                casePoint(x, y);
            } else {
                tmp_thread.x2 = x;
                tmp_thread.y2 = y;
                addThread(tmp_thread);
                drawThread(tmp_thread);
            }
            ++cnt_thread;
            break;
        case TRIANGLE:
            tmp_triangle.color = color;
            if (cnt_triangle % 3 == 0) {
                tmp_triangle.x1 = x;
                tmp_triangle.y1 = y;
                casePoint(x, y);
            } else if (cnt_triangle % 3 == 1) {
                tmp_triangle.x2 = x;
                tmp_triangle.y2 = y;
                tmp_thread.x1 = tmp_triangle.x1;
                tmp_thread.y1 = tmp_triangle.y1;
                tmp_thread.x2 = x;
                tmp_thread.y2 = y;
                tmp_thread.color = color;
                drawThread(tmp_thread);
            } else {
                tmp_triangle.x3 = x;
                tmp_triangle.y3 = y;
                drawTriangle(tmp_triangle);
                addTriangle(tmp_triangle);
            }
            ++cnt_triangle;
            break;
        case RECTANGLE:
            tmp_rectangle.color = color;
            if (cnt_rectangle % 2 == 0) {
                tmp_rectangle.x1 = x;
                tmp_rectangle.y1 = y;
                casePoint(x, y);
            } else {
                tmp_rectangle.x2 = x;
                tmp_rectangle.y2 = y;
                drawRectangle(tmp_rectangle);
                addRectangle(tmp_rectangle);
            }
            ++cnt_rectangle;
            break;
        case CIRCLE:
            tmp_circle.color = color;
            if (cnt_circle % 2 == 0) {
                tmp_circle.x = x;
                tmp_circle.y = y;
                casePoint(x, y);
            } else {
                x -= tmp_circle.x;
                y -= tmp_circle.y;
                tmp_circle.r = sqrt(x * x + y * y);
                drawCircle(tmp_circle);
                addCircle(tmp_circle);
            }
            ++cnt_circle;
            break;
    }
    glFlush();
}