#include "shape.h"
#include "draw.h"
#include "window.h"
#include "cut.h"

#include <cstdlib>
#include <set>
#include <list>
#include <utility>

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

void ThreadShape::cut() {
    clear();
    point p1 = point(this->x1, this->y1), p2 = point(this->x2, this->y2);
    Cut::lineCut(p1, p2);
    this->x1 = p1.first, this->y1 = p1.second;
    this->x2 = p2.first, this->y2 = p2.second;
}

void ThreadShape::save(ostream &out) {
    Shape::save(out);
    out.WRITE(x1).WRITE(y1).WRITE(x2).WRITE(y2);
}

ThreadShape::ThreadShape(Shape *s, istream &in) : Shape(s) {
    in.READ(x1).READ(y1).READ(x2).READ(y2);
    nextX2 = nextY2 = -1;
}

bool ThreadShape::getVertex(int i, int &px, int &py) const {
    switch (i) {
        case 0:
            px = x1, py = y1;
            return true;
        case 1:
            px = x2, py = y2;
            return true;
        default:
            return false;
    }
}

void ThreadShape::setVertex(int i, int px, int py) {
    switch (i) {
        case 0:
            x1 = px, y1 = py;
            break;
        case 1:
            x2 = px, y2 = py;
            break;
        default:
            break;
    }
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
                   points.rbegin()->first, points.rbegin()->second);
    } else {
        if (points.size() == 0)
            return;
//        win.toggleTmpMode();
        if (points.size() == 1)
            Draw::point(points[0].first, points[0].second);
        else {
            for (int i = 0; i < (int) points.size() - 1; ++i) {
                point1 = points[i];
                point2 = points[i + 1];
                Draw::line(point1.first, point1.second, point2.first, point2.second);
            }
        }
//        win.toggleTmpMode();
    }
}

void PolygonShape::doDrawLast() {
    clear();
    if (points.size() < 2) {
        if (points.size() == 1 && nextX >= 0 && nextY >= 0) {
            points.emplace_back(nextX, nextY);
            nextX = nextY = -1;
        } else {
            return;
        }
    }
    win.toggleTmpMode();
//    auto it = points.end();
//    auto it1 = --it;
//    auto it2 = --it;
    auto it1 = points.rbegin();
    if (nextX >= 0 && nextY >= 0) {
//        win.toggleEraseMode();
//        Draw::line((*it1).first, (*it1).second, (*it2).first, (*it2).second);
//        win.toggleEraseMode();
        (*it1).first = nextX;
        (*it1).second = nextY;
        nextX = nextY = -1;
    }
//    Draw::line((*it1).first, (*it1).second, (*it2).first, (*it2).second);
    doDraw();
    win.toggleTmpMode();
}

void PolygonShape::cut() {
    clear();
    vector<Point> src;
    for (auto &i: points)
        src.emplace_back(i);
    points.clear();
    Cut::polygonCut(src, points);
}

void PolygonShape::save(ostream &out) {
    Shape::save(out);
    int sz = (int)points.size();
    out.WRITE(complete).WRITE(sz);
    for (int i = 0; i < (int) points.size(); ++i) {
        out.WRITE(points[i].first).WRITE(points[i].second);
    }
}

PolygonShape::PolygonShape(Shape *s, istream &in) : Shape(s) {
    int sz, x, y;
    in.READ(complete).READ(sz);
    for (int i = 0; i < sz; ++i) {
        in.READ(x).READ(y);
        points.emplace_back(x, y);
    }
    nextY = nextX = -1;
}

bool PolygonShape::getVertex(int i, int &px, int &py) const {
    int sz = (int) points.size();
    if (i < 0 || i >= sz) return false;
    px = points[i].first;
    py = points[i].second;
    return true;
}

void PolygonShape::setVertex(int i, int px, int py) {
    int sz = (int) points.size();
    if (i < 0 || i >= sz) return;
    points[i].first = px;
    points[i].second = py;
    return;
}

void PencilShape::doDraw() {
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

void PencilShape::doDrawLast() {
    if (points.size() > 1) {
        auto it = --points.end(), it2 = it - 1;
        Draw::line(it->first, it->second, it2->first, it2->second);
    } else if (points.size() == 1)
        Draw::point(points[0].first, points[0].second);
}

void PencilShape::cut() {
    bool continues = false;
    std::vector<std::vector<std::pair<int, int>>> pointsSec;
    for (auto &point : points) {
        if (Cut::pointCut(point)) {
            if (!continues) {
                continues = true;
                pointsSec.emplace_back();
            }
            pointsSec.rbegin()->emplace_back(point);
        }
        else {
            continues = false;
        }
    }
    clear();
    if (pointsSec.empty())
        points.clear();
    else
        points = pointsSec[0];
    for (int i = 1; i < (int)pointsSec.size(); ++i) {
        Shape::push(new PencilShape(pointsSec[i]));
    }
}

void PencilShape::save(ostream &out) {
    Shape::save(out);
    int sz = (int)points.size();
    out.WRITE(sz);
    for (int i = 0; i < (int)points.size(); ++i)
        out .WRITE( points[i].first) .WRITE( points[i].second);
}

PencilShape::PencilShape(Shape *s, istream &in) : Shape(s) {
    int sz, x, y;
    in.READ(sz);
    for (int i = 0; i < sz; ++i) {
        in.READ(x).READ(y);
        points.emplace_back(x, y);
    }
}

bool PencilShape::getVertex(int i, int &px, int &py) const {
    int sz = (int) points.size();
    if (i < 0 || i >= sz) return false;
    px = points[i].first;
    py = points[i].second;
    return true;
}

void PencilShape::setVertex(int i, int px, int py) {
    int sz = (int) points.size();
    if (i < 0 || i >= sz) return;
    points[i].first = px;
    points[i].second = py;
    return;
}

bool PencilShape::getVertex(int n, int &x, int &y) {
    if (n >= getVertexNum())
        return false;
    x = points[n].first;
    y = points[n].second;
    return true;
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

CircleShape::CircleShape(int px1, int py1, int px2, int py2) : Shape(S_CIRCLE) {
    x = px1, y = py1;
    px2 -= px1;
    py2 -= py1;
    r = (float) sqrt(px2 * px2 + py2 * py2);
}

CircleShape::CircleShape(const PointShape &mid, const PointShape &p) : Shape(S_CIRCLE) {
    int tmpx, tmpy;
    mid.getVertex(0, x, y);
    p.getVertex(0, tmpx, tmpy);
    tmpx -= x;
    tmpy -= y;
    r = (float) sqrt(tmpx * tmpx + tmpy * tmpy);
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

void EllipseShape::ellipseInit() {
    midx = (x1 + x2) / 2, midy = (y1 + y2) / 2;
    a = abs(x1 - x2) / 2, b = abs(y1 - y2) / 2;
    nextX2 = nextY2 = -1;
}

EllipseShape::EllipseShape(Shape *s, istream &in) : Shape(s) {
    in.READ(x1).READ(y1).READ(x2).READ(y2);
    nextX2 = nextY2 = -1;
    ellipseInit();
}

EllipseShape::EllipseShape(int px1, int py1, int px2, int py2) :
        Shape(S_ELLIPSE), x1(px1), y1(py1), x2(px2), y2(py2) {
    nextX2 = nextY2 = -1;
    ellipseInit();
}

bool EllipseShape::getVertex(int i, int &px, int &py) const {
    switch (i) {
        case 0:
            px = x1, py = y1;
            return true;
        case 1:
            px = x2, py = y2;
            return true;
        default:
            return false;
    }
}

void EllipseShape::setVertex(int i, int px, int py) {
    switch (i) {
        case 0:
            x1 = px, y1 = py;
            break;
        case 1:
            x2 = px, y2 = py;
            break;
        default:
            break;
    }
    ellipseInit();
}


void Shape::clear() {
    win.toggleTmpMode();
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
    win.toggleTmpMode();
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
    PencilShape::doDraw();
    win.toggleEraseMode();
}

void EraserShape::doDrawLast() {
    win.toggleEraseMode();
    setColor(Color::getClearColorIndex());
    PencilShape::doDrawLast();
    win.toggleEraseMode();
}

bool EraserTotalShape::addErasePos(int x, int y) {
    Shape *s = win.read(x, y);
    if (s)
        disabledShapes.push_back(s);
    return s != NULL;
}

void EraserTotalShape::save(ostream &out) {
    Shape::save(out);
    int sz = (int)disabledShapes.size();
    out.WRITE(sz) ;
    for (int i = 0; i < (int)disabledShapes.size(); ++i)
        out.WRITE(shapeDict[disabledShapes[i]]);
}

EraserTotalShape::EraserTotalShape(Shape *s, istream &in) : Shape(s) {
    int sz, j;
    in.READ(sz);
    for (int i = 0; i < sz; ++i) {
        in.READ(j);
        disabledShapes.push_back(shapes[j]);
    }
}

void EraserTotalShape::doDraw() {
    for (auto &i: disabledShapes) {
        if (i->isEnabled()) {
            i->toggleEnableState();
        }
    }
}

void EraserTotalShape::doDrawLast() {
    if (disabledShapes.size()) {
        auto it = --disabledShapes.end();
        if ((*it)->isEnabled())
            (*it)->toggleEnableState();
    }
}

void EraserTotalShape::clear() {
    for (auto &i: disabledShapes) {
        if (i->isEnabled())
            continue;
        i->toggleEnableState();
    }
}

int Shape::curType = S_POINT;
int Shape::curThick = T_MIDDLE;
bool Shape::isCurveShape;
map<Shape *, int> Shape::shapeDict;

const char *const Shape::strShapes[] = {
        "Pencil", "Line", "Polygon", "Circle", "Ellipse", "Curve"
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

Shape *Shape::getShapeFromStream(istream &in) {
    int tmpType = -1, tmpColor, tmpThick;
    bool tmpIsEnable;
    in.READ(tmpType).READ(tmpColor).READ(tmpThick).READ(tmpIsEnable);
    Shape shape(tmpType, tmpColor, tmpThick, tmpIsEnable);
    switch (tmpType) {
        case S_POINT:
            return new PencilShape(&shape, in);
        case S_THREAD:
            return new ThreadShape(&shape, in);
        case S_POLYGON:
            return new PolygonShape(&shape, in);
        case S_CIRCLE:
            return new CircleShape(&shape, in);
        case S_ELLIPSE:
            return new EllipseShape(&shape, in);
        case S_CURVE:
            return new CurveShape(&shape, in);
        case S_ERASER:
            return new EraserShape(&shape, in);
        case S_ERASER_TOTAL:
            return new EraserTotalShape(&shape, in);
        case S_FILL:
            return new FillShape(&shape, in);
        default:
            return NULL;
    }
}

void Shape::pop() {
    if (!shapes.empty()) {
        auto it = shapeDict.find(*shapes.rbegin());
        if (it != shapeDict.end())
            shapeDict.erase(it);
        delete (*shapes.rbegin());
        shapes.pop_back();
    }
}

void Shape::erase(int i) {
    if (i < 0 || i >= (int)shapes.size())
        return;
    if (i == (int)shapes.size())
        pop();
    else {
        auto it = shapes.begin() + i;
        auto it2 = shapeDict.find(*it);
        if (it2 != shapeDict.end())
            shapeDict.erase(it2);
        delete (*it);
        shapes.erase(it);
    }
}

std::vector<Shape *> Shape::shapes;

void FillShape::doDraw() {
    Draw::setFillVertice(xd, yd);
    Draw::fill(x, y);
}

void FillShape::doDrawLast() {
    doDraw();
}

void CurveShape::doDraw() {
    if (points.size() < 2) return;
    else if (points.size() == 2)
        Draw::line(points.begin()->first, points.begin()->second,
                   points.rbegin()->first, points.rbegin()->second);
    else {
        Draw::curve(points);
    }
}

void CurveShape::doDrawLast() {
    clear();
    if (points.size() < 2) {
        if (points.size() == 1 && nextX >= 0 && nextY >= 0) {
            points.emplace_back(nextX, nextY);
            nextX = nextY = -1;
        } else {
            return;
        }
    }
    if (nextX >= 0 && nextY >= 0) {
        if (points.size() <= 2) {
            points[points.size() - 1].first = nextX;
            points[points.size() - 1].second = nextY;
        }
        else {
            points[points.size() - 2].first = nextX;
            points[points.size() - 2].second = nextY;
        }
        nextX = nextY = -1;
    }
    win.toggleTmpMode();
    doDraw();
    win.toggleTmpMode();
}

void CurveShape::addVertex(int x, int y) {
    clear();
    points.emplace_back(x, y);
    if (points.size() >= 3)
        std::swap(points[points.size() - 1], points[points.size() - 2]);
}
