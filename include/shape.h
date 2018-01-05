#ifndef CG_SHAPE_H
#define CG_SHAPE_H

#include "color.h"

#include <vector>
#include <utility>
#include <cmath>
#include <fstream>
#include <map>

using namespace std;
#define WRITE(x) write(reinterpret_cast<const char*>(&x), sizeof x)
#define READ(x) read(reinterpret_cast<char*>(&x), sizeof x)

class Shape {
private:
    const int type;
    int color;
    int thick;
    bool isEnable;
    static int curType;
    static int curThick;
    static bool isCurveShape;

public:
    static map<Shape *, int> shapeDict;
    static std::vector<Shape *> shapes;
    static const int NR_SHAPES = 6;
    static const char *const strShapes[NR_SHAPES];
    static const int NR_THICKS = 3;
    static const char *const strThicks[NR_THICKS];
    static const int NR_ERASERS = 2;
    static const char *const strErasers[NR_ERASERS];
    enum SHAPES {
        S_POINT, S_THREAD, S_POLYGON, S_CIRCLE, S_ELLIPSE, S_CURVE, S_ERASER, S_ERASER_TOTAL, S_FILL, S_CUT, S_TRANS
    };
    enum THICKS {
        T_SMALL, T_MIDDLE, T_BIG
    };

protected:
    virtual void doDraw() {};
    virtual void doDrawLast() {};

public:
    virtual void save(std::ostream &out) {
        out.WRITE(type).WRITE(color).WRITE(thick).WRITE(isEnable);
    };

public:
    Shape(int t) : type(t) {
        color = Color::getCurColor();
        thick = curThick;
        isEnable = true;
    }

    virtual ~Shape() {}

    virtual void clear();

    virtual bool getVertex(int i, int &x, int &y) const { return false; };

    virtual void setVertex(int i, int x, int y) {};

    virtual void cut() {};

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
    Shape(int t, int c, int th, bool ie): type(t), color(c), thick(th), isEnable(ie) {}
    Shape(Shape *s): type(s->type), color(s->color), thick(s->thick), isEnable(s->isEnable) {}
    static Shape *getShapeFromStream(istream &in);
    static void drawAll();

    static void setCurType(int s) {
        curType = s;
    }

    static int getCurType() {
        return curType;
    }

    static void clearAll() {
        shapes.clear();
        shapeDict.clear();
    }

    static void push(Shape *s) {
        shapeDict[s] = (int)shapes.size();
        shapes.push_back(s);
    }

    static void pop();

    static Shape *getShape(int i) {
        if (i < 0 || i >= (int)shapes.size())
            return NULL;
        return shapes[i];
    }

    static void erase(int i);

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
            return *shapes.rbegin();
        return NULL;
    }

    static bool isCurve() { return isCurveShape; }

    static void setCurve(int type) { isCurveShape = type == S_CURVE; }

};

class PointShape : public Shape {
private:
    int x, y;

public:
    void save(ofstream &out) {
        Shape::save(out);
        out.WRITE(x).WRITE(y);
    }
    PointShape(Shape *s, istream &is): Shape(s) {
        is.READ(x).READ(y);
    }
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

class PencilShape : public Shape {
private:
    std::vector<std::pair<int, int>> points;

protected:
    PencilShape(int) : Shape(S_ERASER) {}

public:
    void save(ostream &out);
    virtual void cut();
    PencilShape(Shape *s, istream &in);
    PencilShape() : Shape(S_POINT) {}
    PencilShape(std::vector<std::pair<int, int>> &ps) : Shape(S_POINT) {
        points = std::move(ps);
    }

    PencilShape(int x, int y) : Shape(S_POINT) {
        addVertex(x, y);
    }

    virtual ~PencilShape() {
        clear();
    }

    bool getVertex(int i, int &px, int &py) const;

    void setVertex(int i, int px, int py);

    void addVertex(int x, int y) {
        points.push_back(std::make_pair(x, y));
    }

    virtual void doDraw();

    virtual void doDrawLast();

    int getVertexNum() const { return (int) points.size(); }

    bool getVertex(int n, int &x, int &y);
};

class EraserShape : public PencilShape {
public:
    EraserShape() : PencilShape(0) {
        setColor(Color::getClearColorIndex());
    }
    EraserShape(Shape *s, istream &in): PencilShape(s, in) {
        setColor(Color::getClearColorIndex());
    }
    EraserShape(int x, int y) : PencilShape(0) {
        setColor(Color::getClearColorIndex());
        addVertex(x, y);
    }
    void cut() {}

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
    void save(ostream &out);
    EraserTotalShape(Shape *s, istream &in);
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

    void doDraw();

    void doDrawLast();

    void clear();
};

class ThreadShape : public Shape {
private:
    int x1, y1;
    int x2, y2;
    int nextX2, nextY2;

public:
    void save(ostream &out);
    ThreadShape(Shape *s, istream &in);
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

    void cut();

    bool getVertex(int i, int &px, int &py) const;

    void setVertex(int i, int px, int py);

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
    void save(ostream &out) {
        Shape::save(out);
        out.WRITE(x).WRITE(y).WRITE(r);
    }
    CircleShape(Shape *s, istream &in): Shape(s) {
        in.READ(x).READ(y).READ(r);
        nextR = -1;
    }

    CircleShape(int px, int py, float pr) : Shape(S_CIRCLE), x(px), y(py), r(pr) {}

    CircleShape(int px, int py) : Shape(S_CIRCLE), x(px), y(py), r(-1) {}

    CircleShape(int px1, int py1, int px2, int py2);

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

    CircleShape(const PointShape &mid, const PointShape &p);

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
protected:
    std::vector<std::pair<int, int>> points;
    int nextX, nextY;
    bool complete;
    PolygonShape(int): Shape(S_CURVE) {
        nextX = nextY = -1;
        complete = false;
    }

public:
    void save(ostream &out);
    virtual void cut();
    PolygonShape(Shape *s, istream &in);
    PolygonShape() : Shape(S_POLYGON) {
        nextX = nextY = -1;
        complete = false;
    }
    PolygonShape(std::vector<std::pair<int, int>> &p): Shape(S_POLYGON) {
        nextX = nextY = -1;
        complete = true;
        points = p;
    }

    ~PolygonShape() {
        clear();
    }

    bool getVertex(int i, int &px, int &py) const;

    void setVertex(int i, int px, int py);

    virtual void addVertex(int x, int y) {
        points.emplace_back(x, y);
    }

    void setComplete() {
        complete = true;
    }

    void updateLastVertex(int x, int y) {
        nextX = x;
        nextY = y;
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

class CurveShape : public PolygonShape {
    ~CurveShape() { clear(); }
    void doDraw();
    void doDrawLast();
    void addVertex(int x, int y);

public:
    CurveShape(): PolygonShape(0) {}
    CurveShape(Shape *s, istream &in): PolygonShape(s, in) {}
    void cut() {}
};


class EllipseShape : public Shape {
private:
    int x1, y1, x2, y2;
    int midx, midy, a, b;
    int nextX2, nextY2;

    void ellipseInit();

public:
    void save(ostream &out) {
        Shape::save(out);
        out.WRITE(x1).WRITE(y1).WRITE(x2).WRITE(y2);
    }
    EllipseShape(Shape *s, istream &in);
    EllipseShape(int px1, int py1, int px2, int py2);

    EllipseShape(int px1, int py1) :
            Shape(S_ELLIPSE), x1(px1), y1(py1), x2(-1), y2(-1) {
        midx = midy = a = nextX2 = nextY2 = -1;
    }

    ~EllipseShape() {
        clear();
    }

    bool getVertex(int i, int &px, int &py) const;

    void setVertex(int i, int px, int py);

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
    void save(ostream &out) {
        Shape::save(out);
        out.WRITE(x).WRITE(y);
    }
    FillShape(Shape *s, istream &in): Shape(s) {
        in.READ(x).READ(y);
        setThick(T_SMALL, false);
    }
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

#endif