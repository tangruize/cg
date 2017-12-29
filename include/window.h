#ifndef CG_WINDOW_H
#define CG_WINDOW_H

#include "color.h"
#include "shape.h"
#include "coordinate.h"

#include <GL/glut.h>

const int WIDTH = 1024;
const int HEIGHT = 600;

class window {
private:
    struct attribute {
        Shape *topShape;
        int fillSeq;

        attribute() : topShape(NULL), fillSeq(-1) {}
    };

    int fillSeq;
    attribute **attr;
    int width, height;
    int baseX, baseY;
    int maxWidth, maxHeight;
    float halfWidth, halfHeight;
    Shape *curShape;
    bool eraseFlag, tmpDrawFlag, fillFlag;

    bool getThickPoint(int &x, int &y, int seq) {
        switch (Shape::getCurThick()) {
            case Shape::T_BIG:
                if (seq > 8)
                    return false;
                switch (seq) {
                    case 4:
                        --x;
                        return true;
                    case 5:
                        --y;
                        return true;
                    case 6:
                        --y;
                        return true;
                    case 7:
                        ++x;
                        return true;
                    case 8:
                        ++x;
                        return true;
                    default:
                        break;
                }
            case Shape::T_MIDDLE:
                if (seq > 3)
                    return false;
                switch (seq) {
                    case 1:
                        ++x;
                        return true;
                    case 2:
                        ++y;
                        return true;
                    case 3:
                        --x;
                        return true;
                    default:
                        break;
                }
            case Shape::T_SMALL:
                if (seq == 0)
                    return true;
            default:
                break;
        }
        return false;
    }

    void clear(int x, int y) {
        if (!tmpDrawFlag || attr[x][y].topShape == curShape) {
            attr[x][y].topShape = NULL;
            if (fillFlag)
                attr[x][y].fillSeq = fillSeq;
        }

        if (attr[x][y].topShape == NULL) {
            float fx, fy;
            Coordinate::coorTrans(x, y, fx, fy);
            glVertex2f(fx, fy);
        }
    }

public:
    window(int w, int h) : width(w), height(h) {
        maxWidth = width;
        maxHeight = height;
        halfWidth = (float) width / 2;
        halfHeight = (float) height / 2;
        attr = new attribute *[width];
        eraseFlag = false;
        tmpDrawFlag = false;
        fillSeq = 0;
        fillFlag = false;
        baseX = baseY = 0;
        for (int i = 0; i < width; ++i)
            attr[i] = new attribute[height];
    }

    ~window() {
        for (int i = 0; i < width; ++i)
            delete[] attr[i];
        delete[] attr;
    }

    window() : window(WIDTH, HEIGHT) {}

    bool isValid(int x, int y) {
        return !(x < baseX || y < baseY || x >= width || y >= height);
    }

    void getBase(int &x, int &y) {
        x = baseX;
        y = baseY;
    }

    void setBase(int x, int y) {
        if (x >= 0 && y >= 0 && x <= maxWidth && y <= maxHeight) {
            baseX = x;
            baseY = y;
        }
    }

    void setSize(int x, int y) {
        if (x >= baseX && y >= baseY && x <= maxWidth && y <= maxHeight) {
            width = x;
            height = y;
        }
    }

    void toggleEraseMode() {
        eraseFlag = !eraseFlag;
    }

    void toggleTmpMode() {
        tmpDrawFlag = !tmpDrawFlag;
    }

    void toggleFillMode() {
        fillFlag = !fillFlag;
    }

    void setCursor(int mode) {
        if (mode == 1) {
            glutSetCursor(GLUT_CURSOR_CROSSHAIR);
        } else if (mode == 0) {
            glutSetCursor(GLUT_CURSOR_INHERIT);
        } else if (mode == 2) {
            glutSetCursor(GLUT_CURSOR_SPRAY);
        }
    }

    Shape *getShape(int x, int y) {
        if (!isValid(x, y)) return NULL;
        return attr[x][y].topShape;
    }

    void write(int x, int y) {
        if (!isValid(x, y)) return;
        for (int i = 0; getThickPoint(x, y, i); ++i) {
            if (!isValid(x, y))
                continue;
            if (eraseFlag)
                clear(x, y);
            else {
                if (!tmpDrawFlag || attr[x][y].topShape == NULL) {
                    attr[x][y].topShape = curShape;
                    if (fillFlag)
                        attr[x][y].fillSeq = fillSeq;
                }
                float fx, fy;
                Coordinate::coorTrans(x, y, fx, fy);
                glVertex2f(fx, fy);
            }
        }
    }

    void setShapePointer(Shape *s) {
        curShape = s;
    }

    Shape *getShapePointer() const {
        return curShape;
    }

    Shape *read(int x, int y) {
        if (!isValid(x, y)) return NULL;
        return attr[x][y].topShape;
    }

    int readFillSeq(int x, int y);

    void getSize(int &w, int &h) {
        w = width;
        h = height;
    }

    void getHalfSize(float &hw, float &hh) {
        hw = halfWidth;
        hh = halfHeight;
    }

    void beforeDrawPoint() {
        glBegin(GL_POINTS);
    }

    void afterDrawPoint() {
        glEnd();
    }

    void setClearColor(int c = -1) {
        float *color4fv = Color::setClearColor(c);
        if (color4fv)
            glClearColor(color4fv[0], color4fv[1], color4fv[2], color4fv[3]);
    }

    void initColor() {
        setClearColor();
    }

    int setCurColor(int c) {
        int pre;
        float *color4fv = Color::setCurColor(c, pre);
        glColor4fv(color4fv);
        return pre;
    }

    void init(int argc, char *argv[]);

    void display() {
        glClear(GL_COLOR_BUFFER_BIT);
        Shape::drawAll();
        glutSwapBuffers();
        glFlush();
    }

    void flush() {
        glutSwapBuffers();
        glFlush();
    }

    int getFillSeq() {
        return ++fillSeq;
    }
};

extern class window win;

#endif