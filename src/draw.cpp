#include "draw.h"
#include "dda.h"
#include "shape.h"
#include "coordinate.h"
#include "file.h"
#include "circle.h"

#include <GL/glut.h>
#include <vector>

using namespace std;

static vector<point> points;
static vector<thread> threads;
static vector<triangle> triangles;
static vector<rectangle> rectangles;
static vector<circle> circles;

void clearAll() {
    points.clear();
    threads.clear();
    triangles.clear();
    rectangles.clear();
    circles.clear();
}

void load() {
    doLoad(points, threads, triangles, rectangles, circles);
}

void save() {
    doSave(points, threads, triangles, rectangles, circles);
}

void addPoint(const struct point &s) {
    points.push_back(s);
}

void addThread(const struct thread &s) {
    threads.push_back(s);
}

void addTriangle(const struct triangle &s) {
    triangles.push_back(s);
}

void addRectangle(const struct rectangle &s) {
    rectangles.push_back(s);
}

void addCircle(const struct circle &s) {
    circles.push_back(s);
}

static void setColor(int c) {
    switch (c) {
        case BLACK:
            glColor3f(0.0, 0.0, 0.0);
            break;
        case RED:
            glColor3f(1.0, 0.0, 0.0);
            break;
        case GREEN:
            glColor3f(0.0, 1.0, 0.0);
            break;
        case BLUE:
            glColor3f(0.0, 0.0, 1.0);
            break;
    }
}

void drawPoint(const struct point &s) {
    setColor(s.color);
    float fx, fy;
    coorTrans(s.x, s.y, &fx, &fy);
    glBegin(GL_POINTS);
    glVertex2f(fx, fy);
    glEnd();
}

void drawThread(const struct thread &s) {
    setColor(s.color);
    dda(s.x1, s.y1, s.x2, s.y2);
}

void drawTriangle(const struct triangle &s) {
    setColor(s.color);
    dda(s.x1, s.y1, s.x2, s.y2);
    dda(s.x2, s.y2, s.x3, s.y3);
    dda(s.x1, s.y1, s.x3, s.y3);
}

void drawRectangle(const struct rectangle &s) {
    setColor(s.color);
    dda(s.x1, s.y1, s.x1, s.y2);
    dda(s.x1, s.y1, s.x2, s.y1);
    dda(s.x2, s.y2, s.x1, s.y2);
    dda(s.x2, s.y2, s.x2, s.y1);
}

void drawCircle(const struct circle &s) {
    setColor(s.color);
    BresenhamCircle(s.x, s.y, s.r);
}

void drawAllPoints() {
    for (unsigned i = 0; i < points.size(); ++i) {
        drawPoint(points[i]);
    }
}

void drawAlllThreads() {
    for (unsigned i = 0; i < threads.size(); ++i) {
        drawThread(threads[i]);
    }
}

void drawAllTriangles() {
    for (unsigned i = 0; i < triangles.size(); ++i) {
        drawTriangle(triangles[i]);
    }
}

void drawAllRectangles() {
    for (unsigned i = 0; i < rectangles.size(); ++i) {
        drawRectangle(rectangles[i]);
    }
}

void drawAllCircles() {
    for (unsigned i = 0; i < circles.size(); ++i) {
        drawCircle(circles[i]);
    }
}

void drawAllShapes() {
    drawAllPoints();
    drawAlllThreads();
    drawAllTriangles();
    drawAllRectangles();
    drawAllCircles();
}