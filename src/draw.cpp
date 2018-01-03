#include "draw.h"
#include "line.h"
#include "shape.h"
#include "fill.h"
#include "ellipse.h"
#include "curve.h"
#include "window.h"

using namespace std;

void Draw::point(int x, int y) {
    win.beforeDrawPoint();
    win.write(x, y);
    win.afterDrawPoint();
}

void Draw::line(int x1, int y1, int x2, int y2) {
    win.beforeDrawPoint();
    LineAlgo::draw(x1, y1, x2, y2);
    win.afterDrawPoint();
}

void Draw::circle(int x, int y, float r) {
    win.beforeDrawPoint();
    CircleAlgo::draw(x, y, r);
    win.afterDrawPoint();
}

void Draw::ellipse(int x, int y, int a, int b) {
    win.beforeDrawPoint();
    EllipseAlgo::draw(x, y, a, b);
    win.afterDrawPoint();
}

void Draw::fill(int x, int y) {
    win.beforeDrawPoint();
    FillAlgo::draw(x, y);
    win.afterDrawPoint();
}

void Draw::setFillVertice(std::vector<int> &xd, std::vector<int> &yd) {
    FillAlgo::setFillVertice(xd, yd);
}

void Draw::curve(std::vector<std::pair<int, int>> &vertices) {
    win.beforeDrawPoint();
    CurveAlgo::draw(vertices);
    win.afterDrawPoint();
}
