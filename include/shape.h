#ifndef CG_SHAPE_H
#define CG_SHAPE_H

#include "color.h"

#include <vector>
#include <utility>
#include <cmath>

class Shape {
private:
    const int type;
    int color;
    int thick;
    bool isEnable;
    static int curType;
    static int curThick;
    static std::vector<Shape *> shapes;

public:
    static const int NR_SHAPES = 5;
    static const char *const strShapes[NR_SHAPES];
    static const int NR_THICKS = 3;
    static const char *const strThicks[NR_THICKS];
    static const int NR_ERASERS = 2;
    static const char *const strErasers[NR_ERASERS];
    enum SHAPES {
        S_POINT, S_THREAD, S_POLYGON, S_CIRCLE, S_ELLIPSE, S_ERASER, S_ERASER_TOTAL, S_FILL, S_CUT, S_TRANS
    };
    enum THICKS {
        T_SMALL, T_MIDDLE, T_BIG
    };

protected:
    virtual void doDraw() = 0;

    virtual void doDrawLast() = 0;

public:
    Shape(int t) : type(t) {
        color = Color::getCurColor();
        thick = curThick;
        isEnable = true;
    }

    virtual ~Shape() {}

    virtual void clear();

    virtual bool getVertex(int i, int &x, int &y) const = 0;

    virtual void setVertex(int i, int x, int y) = 0;

    void draw();

    void drawLast();

    void setColor(int c) { color = c; }

    void setThick(int t, bool isClear = true) {
        if (isClear)
            clear();
        thick = t;
        if (isClear)
            draw();
    }

    int getColor() const { return color; }

    int getType() const { return type; }

    int getThick() const { return thick; }

    bool isEnabled() const { return isEnable; }

    void toggleEnableState() {
        if (isEnable)
            clear();
        else
            draw();
        isEnable = !isEnable;
    }

public:
    static void drawAll();

    static void setCurType(int s) {
        curType = s;
    }

    static int getCurType() {
        return curType;
    }

    static void clearAll() {
        shapes.clear();
    }

    static void push(Shape *s) {
        shapes.push_back(s);
    }

    static void pop() {
        if (shapes.size()) {
            delete (*--shapes.end());
            shapes.pop_back();
        }
    }

    static Shape *getShape(int i) {
        if (i < 0 || i >= shapes.size())
            return NULL;
        return shapes[i];
    }

    static void erase(int i) {
        if (i < 0 || i >= shapes.size())
            return;
        if (i == shapes.size())
            pop();
        else {
            auto it = shapes.begin() + i;
            delete (*it);
            shapes.erase(it);
        }
    }

    static int setCurThick(int t) {
        int preThick = curThick;
        curThick = t;
        return preThick;
    }

    static int getCurThick() {
        return curThick;
    }

    static int size() {
        return (int) shapes.size();
    }

    static Shape *getLastShape() {
        if (shapes.size())
            return *--shapes.end();
        return NULL;
    }

};

class PointShape : public Shape {
private:
    int x, y;

public:
    PointShape(int px, int py) : Shape(S_POINT), x(px), y(py) {}

    void doDraw();

    void doDrawLast() {
        doDraw();
    }

    ~PointShape() {
        clear();
    }

    bool getVertex(int i, int &px, int &py) const {
        if (i != 0) return false;
        px = x, py = y;
        return true;
    }

    void setVertex(int i, int px, int py) {
        if (i != 0) return;
        x = px, y = py;
    }
};

class CurveShape : public Shape {
private:
    std::vector<std::pair<int, int>> points;

protected:
    CurveShape(int) : Shape(S_ERASER) {}

public:
    CurveShape() : Shape(S_POINT) {}

    CurveShape(int x, int y) : Shape(S_POINT) {
        addVertex(x, y);
    }

    virtual ~CurveShape() {
        clear();
    }

    bool getVertex(int i, int &px, int &py) const {
        int sz = (int) points.size();
        if (i < 0 || i >= sz) return false;
        px = points[i].first;
        py = points[i].second;
        return true;
    }

    void setVertex(int i, int px, int py) {
        int sz = (int) points.size();
        if (i < 0 || i >= sz) return;
        points[i].first = px;
        points[i].second = py;
        return;
    }

    void addVertex(int x, int y) {
        points.push_back(std::make_pair(x, y));
    }

    virtual void doDraw();

    virtual void doDrawLast();

    int getVertexNum() const { return (int) points.size(); }

    bool getVertex(int n, int &x, int &y) {
        if (n >= getVertexNum())
            return false;
        x = points[n].first;
        y = points[n].second;
        return true;
    }
};

class EraserShape : public CurveShape {
public:
    EraserShape() : CurveShape(0) {
        setColor(Color::getClearColorIndex());
    }

    EraserShape(int x, int y) : CurveShape(0) {
        setColor(Color::getClearColorIndex());
        addVertex(x, y);
    }

    ~EraserShape() {
        clear();
    }

    void doDraw();

    void doDrawLast();
};

class EraserTotalShape : public Shape {
private:
    std::vector<Shape *> disabledShapes;
public:
    EraserTotalShape() : Shape(S_ERASER_TOTAL) {}

    EraserTotalShape(int x, int y) : Shape(S_ERASER_TOTAL) {
        addErasePos(x, y);
    }

    ~EraserTotalShape() {
        clear();
    }

    bool getVertex(int i, int &px, int &py) const {
        return false;
    }

    void setVertex(int i, int px, int py) {
        return;
    }

    bool addErasePos(int x, int y);

    void doDraw() {
        for (auto &i: disabledShapes) {
            if (i->isEnabled()) {
                i->toggleEnableState();
            }
        }
    }

    void doDrawLast() {
        if (disabledShapes.size()) {
            auto it = --disabledShapes.end();
            if ((*it)->isEnabled())
                (*it)->toggleEnableState();
        }
    }

    void clear() {
        for (auto &i: disabledShapes) {
            if (i->isEnabled())
                continue;
            i->toggleEnableState();
        }
    }
};

class ThreadShape : public Shape {
private:
    int x1, y1;
    int x2, y2;
    int nextX2, nextY2;

public:
    ThreadShape(int px1, int py1, int px2, int py2) :
            Shape(S_THREAD), x1(px1), y1(py1), x2(px2), y2(py2) {
        nextX2 = nextY2 = -1;
    }

    ThreadShape(const PointShape &p1, const PointShape &p2) : Shape(S_THREAD) {
        p1.getVertex(0, x1, y1);
        p2.getVertex(0, x2, y2);
    }

    ThreadShape(int px1, int py1) :
            Shape(S_THREAD), x1(px1), y1(py1), x2(-1), y2(-1) {
        nextX2 = nextY2 = -1;
    }

    ~ThreadShape() {
        clear();
    }

    bool getVertex(int i, int &px, int &py) const {
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

    void setVertex(int i, int px, int py) {
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

    void doDraw();

    void doDrawLast();

    void updateVertex2(int x, int y);

    void getVertex(int &px1, int &py1, int &px2, int &py2) const {
        px1 = x1, py1 = y1;
        px2 = x2, py2 = y2;
    }
};

class CircleShape : public Shape {
private:
    int x, y;
    float r, nextR;

public:
    CircleShape(int px, int py, float pr) : Shape(S_CIRCLE), x(px), y(py), r(pr) {}

    CircleShape(int px, int py) : Shape(S_CIRCLE), x(px), y(py), r(-1) {}

    CircleShape(int px1, int py1, int px2, int py2) : Shape(S_CIRCLE) {
        x = px1, y = py1;
        px2 -= px1;
        py2 -= py1;
        r = (float) sqrt(px2 * px2 + py2 * py2);
    }

    CircleShape(const PointShape &p, float pr) : Shape(S_CIRCLE) {
        p.getVertex(0, x, y);
        r = pr;
    }

    bool getVertex(int i, int &px, int &py) const {
        if (i != 0) return false;
        px = x, py = y;
        return true;
    }

    void setVertex(int i, int px, int py) {
        if (i != 0) return;
        x = px, y = py;
    }

    CircleShape(const PointShape &mid, const PointShape &p) : Shape(S_CIRCLE) {
        int tmpx, tmpy;
        mid.getVertex(0, x, y);
        p.getVertex(0, tmpx, tmpy);
        tmpx -= x;
        tmpy -= y;
        r = (float) sqrt(tmpx * tmpx + tmpy * tmpy);
    }

    ~CircleShape() {
        clear();
    }

    void doDraw();

    void doDrawLast();

    void updateR(int x, int y);

    void getVertex(int &px, int &py, float &pr) const {
        px = x, py = y;
        pr = r;
    }
};

class PolygonShape : public Shape {
private:
    std::vector<std::pair<int, int>> points;
    int nextX, nextY;
    bool complete;

public:
    PolygonShape() : Shape(S_POLYGON) {
        nextX = nextY = -1;
        complete = false;
    }

    ~PolygonShape() {
        clear();
    }

    bool getVertex(int i, int &px, int &py) const {
        int sz = (int) points.size();
        if (i < 0 || i >= sz) return false;
        px = points[i].first;
        py = points[i].second;
        return true;
    }

    void setVertex(int i, int px, int py) {
        int sz = (int) points.size();
        if (i < 0 || i >= sz) return;
        points[i].first = px;
        points[i].second = py;
        return;
    }

    void addVertex(int x, int y) {
        points.push_back(std::make_pair(x, y));
    }

    void setComplete() {
        complete = true;
    }

    void updateLastVertex(int x, int y) {
        nextX = x;
        nextY = y;
    }

    void doDraw();

    void doDrawLast();

    int getVertexNum() const { return (int) points.size(); }

    bool getVertex(int n, int &x, int &y) {
        if (n >= getVertexNum())
            return false;
        x = points[n].first;
        y = points[n].second;
        return true;
    }
};

class EllipseShape : public Shape {
private:
    int x1, y1, x2, y2;
    int midx, midy, a, b;
    int nextX2, nextY2;

    void ellipseInit() {
        midx = (x1 + x2) / 2, midy = (y1 + y2) / 2;
        a = abs(x1 - x2) / 2, b = abs(y1 - y2) / 2;
        nextX2 = nextY2 = -1;
    }

public:
    EllipseShape(int px1, int py1, int px2, int py2) :
            Shape(S_ELLIPSE), x1(px1), y1(py1), x2(px2), y2(py2) {
        nextX2 = nextY2 = -1;
        ellipseInit();
    }

    EllipseShape(int px1, int py1) :
            Shape(S_ELLIPSE), x1(px1), y1(py1), x2(-1), y2(-1) {
        midx = midy = a = nextX2 = nextY2 = -1;
    }

    ~EllipseShape() {
        clear();
    }

    bool getVertex(int i, int &px, int &py) const {
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

    void setVertex(int i, int px, int py) {
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

    void doDraw();

    void doDrawLast();

    void updateVertex2(int x, int y);

    void getVertex(int &px1, int &py1, int &px2, int &py2) const {
        px1 = x1, py1 = y1;
        px2 = x2, py2 = y2;
    }

    void getPara(int &pmidx, int &pmidy, int &pa, int &pb) {
        pmidx = midx, pmidy = midy;
        pa = a, pb = b;
    }
};

class FillShape : public Shape {
private:
    int x, y;
    std::vector<int> xd;
    std::vector<int> yd;

public:
    FillShape(int px, int py) : Shape(S_FILL), x(px), y(py) {
        setThick(T_SMALL, false);
    }

    ~FillShape() {
        clear();
    }

    bool getVertex(int i, int &px, int &py) const {
        if (i != 0) return false;
        px = x, py = y;
        return true;
    }

    void setVertex(int i, int px, int py) {
        if (i != 0) return;
        x = px, y = py;
    }

    void doDraw();

    void doDrawLast();
};

class CutShape : public Shape {
private:
    int baseX, baseY, width, height;
    int preBaseX, preBaseY, preWidth, preHeight;
public:
    CutShape(int x, int y);

    ~CutShape() {
        clear();
    }

    bool getVertex(int i, int &px, int &py) const {
        return false;
    }

    void setVertex(int i, int px, int py) {
        return;
    }

    void clear();

    void doDraw();

    void doDrawLast();

    void updateVertex2(int x, int y);
};

#endif