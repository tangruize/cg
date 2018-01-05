//
// Created by tangruize on 18-1-3.
//

#include "cut.h"
#include "shape.h"
#include "window.h"
#include <set>

using namespace std;

static set<Shape *> shapeSet;

enum {LEFT, RIGHT, BOTTOM, TOP};

static const unsigned char LEFT_CODE   = 01;
static const unsigned char RIGHT_CODE  = 02;
static const unsigned char BOTTOM_CODE = 04;
static const unsigned char TOP_CODE    = 010;

static int x_min, x_max, y_min, y_max;

void horizontal(int x, int y, int end) {
    while (y <= end) {
        Shape *s = win.read(x, y);
        if (s)
            shapeSet.insert(s);
        ++y;
    }
}

void vertical(int x, int y, int end) {
    while (x <= end) {
        Shape *s = win.read(x, y);
        if (s)
            shapeSet.insert(s);
        ++x;
    }
}

void Cut::cut(int x1, int y1, int x2, int y2) {
    shapeSet.clear();
    x_min = min(x1, x2), y_min = min(y1, y2);
    x_max = max(x1, x2), y_max = max(y1, y2);
    horizontal(x_min, y_min, y_max);
    horizontal(x_max, y_min, y_max);
    vertical(x_min, y_min, x_max);
    vertical(x_min, y_max, x_max);
    for (auto &s: shapeSet) {
        s->cut();
    }
}

static void setPointLoc(Point* pr) {
    pr->loc = 0;
    if (pr->x < x_min)
        pr->loc |= LEFT_CODE;
    else if (pr->x > x_max)
        pr->loc |= RIGHT_CODE;
    if (pr->y < y_min)
        pr->loc |= BOTTOM_CODE;
    else if (pr->y > y_max)
        pr->loc |= TOP_CODE;
}

static bool isInnerPoint(Point p, const int mode) {
    setPointLoc(&p);
    switch (mode) {
        case LEFT:
            return (p.loc & LEFT_CODE) != LEFT_CODE;
        case RIGHT:
            return (p.loc & RIGHT_CODE) != RIGHT_CODE;
        case BOTTOM:
            return (p.loc & BOTTOM_CODE) != BOTTOM_CODE;
        case TOP:
            return (p.loc & TOP_CODE) != TOP_CODE;
        default:
            return false;
    }
}

static void horizontalCut(const Point &p1, const Point &p2, vector<Point> &points, const int mode) {
    switch (mode) {
        case LEFT:
            if ((p1.x >= x_min) && (p2.x >= x_min))
                points.push_back(p2);
            else if ((p1.x < x_min) && (p2.x >= x_min)) {
                Point temp(x_min, p1.y);
                points.push_back(temp);
                points.push_back(p2);
            }
            else if ((p1.x >= x_min) && (p2.x < x_min)) {
                points.push_back(p1);
                Point temp(x_min, p1.y);
                points.push_back(temp);
            }
            break;
        case RIGHT:
            if ((p1.x <= x_max) && (p2.x <= x_max))
                points.push_back(p2);
            else if ((p1.x <= x_max) && (p2.x > x_max)) {
                points.push_back(p1);
                Point temp(x_max, p1.y);
                points.push_back(temp);
            }
            else if ((p1.x > x_max) && (p2.x <= x_max)) {
                Point temp(x_max, p1.y);
                points.push_back(temp);
                points.push_back(p2);
            }
            break;
        case BOTTOM:
            if (p1.y >= y_min)
                points.push_back(p2);
            break;
        case TOP:
            if (p1.y <= y_max)
                points.push_back(p2);
            break;
        default:
            break;
    }
}

static void verticalCut(const Point &p1, const Point &p2, vector<Point> &points, const int mode) {
    switch (mode) {
        case LEFT:
            if (p1.x >= x_min)
                points.push_back(p2);
            break;
        case RIGHT:
            if (p1.x <= x_max)
                points.push_back(p2);
            break;
        case BOTTOM:
            if ((p1.y >= y_min) && (p2.y >= y_min))
                points.push_back(p2);
            else if ((p1.y >= y_min) && (p2.y < y_min)) {
                Point temp(p1.x, y_min);
                points.push_back(temp);
            }
            else if ((p1.y < y_min) && (p2.y >= y_min)) {
                Point temp(p1.x, y_min);
                points.push_back(temp);
                points.push_back(p2);
            }
            break;
        case TOP:
            if ((p1.y <= y_max) && (p2.y <= y_max))
                points.push_back(p2);
            else if ((p1.y > y_max) && (p2.y <= y_max)) {
                Point temp(p1.x, y_max);
                points.push_back(temp);
                points.push_back(p2);
            }
            else if ((p1.y <= y_max) && (p2.y > y_max)) {
                Point temp(p1.x, y_max);
                points.push_back(temp);
            }
            break;
        default:
            break;
    }
}

static void twoWayCut(const Point &p1, const Point &p2, vector<Point> &points, const int mode) {
    if (p1.x == p2.x)
        verticalCut(p1, p2, points, mode);
    else if (p1.y == p2.y)
        horizontalCut(p1, p2, points, mode);
}

static Point crossPoint(const Point &p1, const Point &p2, const int mode) {
    Point result;
    switch (mode) {
        case LEFT:
            result.x = x_min;
            result.y = static_cast<int>(p1.y + (x_min - p1.x) * (p2.y - p1.y) / (double)(p2.x - p1.x));
            break;
        case RIGHT:
            result.x = x_max;
            result.y = static_cast<int>(p1.y + (x_max - p1.x) * (p2.y - p1.y) / (double)(p2.x - p1.x));
            break;
        case BOTTOM:
            result.y = y_min;
            result.x = static_cast<int>(p1.x + (y_min - p1.y) / ((double)(p2.y - p1.y) / (double)(p2.x - p1.x)));
            break;
        case TOP:
            result.y = y_max;
            result.x = static_cast<int>(p1.x + (y_max - p1.y) / ((double)(p2.y - p1.y) / (double)(p2.x - p1.x)));
            break;
        default:
            break;
    }
    return result;
}

static void doPolygonCut(vector<Point> &src, vector<Point> &target, const int mode) {
    if (src.empty())
        return;
    for (auto pl = src.begin(); pl != src.end(); ++pl) {
        auto pr = pl + 1;
        if (pr == src.end())
            break;
        if (isInnerPoint(*pl, mode)) {
            if (isInnerPoint(*pr, mode))
                target.push_back(*pr);
            else
                target.push_back(crossPoint(*pl, *pr, mode));
        }
        else if (isInnerPoint(*pr, mode)) {
            target.push_back(crossPoint(*pl, *pr, mode));
            target.push_back(*pr);
        }

    }

    if (src.rbegin()->x == src.begin()->x || src.rbegin()->y == src.begin()->y)
        twoWayCut(*src.rbegin(), *src.begin(), target, mode);
    else if (isInnerPoint(*src.rbegin(), mode)) {
        if (isInnerPoint(*src.begin(), mode))
            target.push_back(*src.begin());
        else if (!isInnerPoint(*src.begin(), mode))
            target.push_back(crossPoint(*src.rbegin(), *src.begin(), mode));
    }
    else if (isInnerPoint(*src.begin(), mode)) {
        target.push_back(crossPoint(*src.rbegin(), *src.begin(), mode));
        target.push_back(*src.begin());
    }
}

void Cut::polygonCut(vector<Point> &src, vector<point> &result) {
    vector<Point> pLeft, pRight, pBottom, pResult;
    doPolygonCut(src, pLeft, LEFT);
    doPolygonCut(pLeft, pRight, RIGHT);
    doPolygonCut(pRight, pBottom, BOTTOM);
    doPolygonCut(pBottom, pResult, TOP);
    for (auto &i: pResult)
        result.emplace_back(i.x, i.y);
    if (pResult.empty())
        for (auto &i: src)
            result.emplace_back(i.x, i.y);
}

static bool checkArea(int x, int y) {
    return !(x < x_min || y < y_min || x > x_max || y > y_max);
}

static bool checkArea(int x1, int y1, int x2, int y2, int x, int y) {
    int minx = min(x1, x2), miny = min(y1, y2);
    int maxx = max(x1, x2), maxy = max(y1, y2);
    return !(x < minx || y < miny || x > maxx || y > maxy);
}

static bool getCrossPoint(point &p1, point &p2, int check, bool isX, point &dst) {
    int x = p1.first - p2.first, y = p1.second - p2.second;
    double fx, fy;
    if (isX) {
        if (x == 0)
            return false;
        fx = check;
        check = y * (check - p1.first);
        fy = (double)check / x + p1.second;
    }
    else {
        if (y == 0)
            return false;
        fy = check;
        check = x * (check - p1.second);
        fx = (double)check / y + p1.first;
    }
    dst.first = (int)fx;
    dst.second = (int)fy;
    return checkArea(p1.first, p1.second, p2.first, p2.second, dst.first, dst.second);
}

void Cut::lineCut(point &src_p1, point &src_p2) {
    point dst, p1 = point(src_p1.first, src_p1.second), p2 = point(src_p2.first, src_p2.second);
    std::set<point> cross;
    if (checkArea(src_p1.first, src_p1.second))
        cross.insert(point(src_p1.first, src_p1.second));
    if (checkArea(src_p2.first, src_p2.second))
        cross.insert(point(src_p2.first, src_p2.second));
    if (cross.size() == 2)
        return;
    if (getCrossPoint(p1, p2, x_min, true, dst)) {
        if (checkArea(dst.first, dst.second))
            cross.insert(dst);
    }
    if (cross.size() < 2 && getCrossPoint(p1, p2, x_max, true, dst)) {
        if (checkArea(dst.first, dst.second))
            cross.insert(dst);
    }
    if (cross.size() < 2 && getCrossPoint(p1, p2, y_min, false, dst)) {
        if (checkArea(dst.first, dst.second))
            cross.insert(dst);
    }
    if (cross.size() < 2 && getCrossPoint(p1, p2, y_max, false, dst)) {
        if (checkArea(dst.first, dst.second))
            cross.insert(dst);
    }
    if (cross.size() != 2)
        return;
    auto it = cross.begin();
    src_p1.first = it->first;
    src_p1.second = it->second;
    src_p2.first = (++it)->first;
    src_p2.second = it->second;
}

bool Cut::pointCut(const point &p) {
    return checkArea(p.first, p.second);
}
