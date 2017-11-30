#include "shape.h"
#include "draw.h"
#include "window.h"

#include <cstdlib>

using namespace std;

void PointShape::doDraw() {
    Draw::point(x, y);
}

void ThreadShape::doDraw() {
    if (x2 >= 0)
        Draw::line(x1, y1, x2, y2);
    else
        Draw::point(x1, y1);
}

void ThreadShape::doDrawLast() {
    if (nextX2 >= 0 && nextY2 >= 0) {
        clear();
        x2 = nextX2;
        y2 = nextY2;
        nextX2 = nextY2 = -1;
    }
    win.toggleTmpMode();
    doDraw();
    win.toggleTmpMode();
}

void ThreadShape::updateVertex2(int x, int y) {
    nextX2 = x;
    nextY2 = y;
}

void PolygonShape::doDraw() {
    pair<int, int> point1, point2;
    if (complete && points.size() >= 3) {
        for (int i = 0; i < (int) points.size() - 1; ++i) {
            point1 = points[i];
            point2 = points[i + 1];
            Draw::line(point1.first, point1.second, point2.first, point2.second);
        }
        Draw::line(points.begin()->first, points.begin()->second,
                   (--points.end())->first, (--points.end())->second);
    } else {
        if (points.size() == 0)
            return;
        win.toggleTmpMode();
        if (points.size() == 1)
            Draw::point(points[0].first, points[0].second);
        else {
            for (int i = 0; i < (int) points.size() - 1; ++i) {
                point1 = points[i];
                point2 = points[i + 1];
                Draw::line(point1.first, point1.second, point2.first, point2.second);
            }
        }
        win.toggleTmpMode();
    }
}

void PolygonShape::doDrawLast() {
    if (points.size() < 2) {
        if (points.size() == 1 && nextX >= 0 && nextY >= 0) {
            points.push_back(std::make_pair(nextX, nextY));
            nextX = nextY = -1;
        } else {
            return;
        }
    }
    win.toggleTmpMode();
    auto it = points.end();
    auto it1 = --it;
    auto it2 = --it;
    if (nextX >= 0 && nextY >= 0) {
        win.toggleEraseMode();
        Draw::line((*it1).first, (*it1).second, (*it2).first, (*it2).second);
        win.toggleEraseMode();
        (*it1).first = nextX;
        (*it1).second = nextY;
        nextX = nextY = -1;
    }
    Draw::line((*it1).first, (*it1).second, (*it2).first, (*it2).second);
    win.toggleTmpMode();
}

void CurveShape::doDraw() {
    if (points.size() > 1) {
        pair<int, int> point1, point2;
        for (int i = 0; i < (int) points.size() - 1; ++i) {
            point1 = points[i];
            point2 = points[i + 1];
            Draw::line(point1.first, point1.second, point2.first, point2.second);
        }
    } else if (points.size() == 1)
        Draw::point(points[0].first, points[0].second);
}

void CurveShape::doDrawLast() {
    if (points.size() > 1) {
        auto it = --points.end(), it2 = it - 1;
        Draw::line(it->first, it->second, it2->first, it2->second);
    } else if (points.size() == 1)
        Draw::point(points[0].first, points[0].second);
}

void CircleShape::doDraw() {
    if (r <= 0)
        return;
    Draw::circle(x, y, r);
}

void CircleShape::doDrawLast() {
    if (nextR >= 0) {
        clear();
        r = nextR;
        nextR = -1;
    }
    win.toggleTmpMode();
    doDraw();
    win.toggleTmpMode();
}

void CircleShape::updateR(int tmpx, int tmpy) {
    tmpx -= x;
    tmpy -= y;
    if (r <= 0)
        r = (float) sqrt(tmpx * tmpx + tmpy * tmpy);
    else
        nextR = (float) sqrt(tmpx * tmpx + tmpy * tmpy);
}

void EllipseShape::doDraw() {
    if (midx >= 0 && midy >= 0 && a > 0 && b > 0)
        Draw::ellipse(midx, midy, a, b);
}

void EllipseShape::doDrawLast() {
    if (nextX2 > 0 && nextY2 > 0) {
        clear();
        x2 = nextX2;
        y2 = nextY2;
        ellipseInit();
        nextX2 = nextY2 = -1;
    }
    win.toggleTmpMode();
    doDraw();
    win.toggleTmpMode();
}

void EllipseShape::updateVertex2(int x, int y) {
    if (midx < 0) {
        x2 = x;
        y2 = y;
        ellipseInit();
    } else {
        nextX2 = x;
        nextY2 = y;
    }
}


void Shape::clear() {
    bool stateChanged = false;
    if (!isEnable)
        stateChanged = isEnable = true;
    int savedColor = color;
    color = Color::C_DESTROY;
    win.toggleEraseMode();
    draw();
    win.toggleEraseMode();
    if (stateChanged)
        isEnable = false;
    color = savedColor;
}

void Shape::drawAll() {
    for (auto &i: shapes) {
        if (i->isEnabled()) {
            i->draw();
        }
    }
}

void EraserShape::doDraw() {
    win.toggleEraseMode();
    setColor(Color::getClearColorIndex());
    CurveShape::doDraw();
    win.toggleEraseMode();
}

void EraserShape::doDrawLast() {
    win.toggleEraseMode();
    setColor(Color::getClearColorIndex());
    CurveShape::doDrawLast();
    win.toggleEraseMode();
}

bool EraserTotalShape::addErasePos(int x, int y) {
    Shape *s = win.read(x, y);
    if (s)
        disabledShapes.push_back(s);
    return s != NULL;
}

int Shape::curType = S_POINT;
int Shape::curThick = T_MIDDLE;

const char *const Shape::strShapes[] = {
        "Curve", "Thread", "Polygon", "Circle", "Ellipse"
};

const char *const Shape::strThicks[] = {
        "Small", "Middle", "Big"
};

const char *const Shape::strErasers[]{
        "Total", "Partial"
};

void Shape::draw() {
    win.setShapePointer(this);
    int drawSavedColor = win.setCurColor(getColor());
    int drawSavedThick = Shape::setCurThick(getThick());

    doDraw();

    if (getColor() != drawSavedColor)
        win.setCurColor(drawSavedColor);
    if (getThick() != drawSavedThick)
        Shape::setCurThick(drawSavedThick);
}

void Shape::drawLast() {
    win.setShapePointer(this);
    int drawSavedColor = win.setCurColor(getColor());
    int drawSavedThick = Shape::setCurThick(getThick());

    doDrawLast();

    if (getColor() != drawSavedColor)
        win.setCurColor(drawSavedColor);
    if (getThick() != drawSavedThick)
        Shape::setCurThick(drawSavedThick);
}

std::vector<Shape *> Shape::shapes;

void FillShape::doDraw() {
    Draw::fill(x, y);
}

void FillShape::doDrawLast() {
    doDraw();
}

CutShape::CutShape(int x, int y) : Shape(S_CUT) {
    win.getBase(preBaseX, preBaseY);
    win.getSize(preWidth, preHeight);
    baseX = x;
    baseY = y;
}

void CutShape::clear() {
    win.setBase(preBaseX, preBaseX);
    win.setSize(preWidth, preHeight);
}

void CutShape::doDraw() {
    win.setBase(baseX, baseY);
    win.setSize(width, height);
}

void CutShape::doDrawLast() {
    doDraw();
}

void CutShape::updateVertex2(int x, int y) {
    width = x, height = y;
}
