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
    float halfWidth, halfHeight;
    Shape *curShape;
    bool eraseFlag, tmpDrawFlag, fillFlag;

    bool getThickPoint(int &x, int &y, int seq, int thickness);

    void clear(int x, int y);

public:
    window(int w, int h);

    ~window() {
       destroyWindow();
    }

    void destroyWindow() {
        for (int i = 0; i < width; ++i)
            delete[] attr[i];
        delete[] attr;
    }

    void createWindow();

    void createWindow(int w, int h) {
        width = w;
        height = h;
        createWindow();
    }

    window() : window(WIDTH, HEIGHT) {}

    bool isValid(int x, int y) {
        return !(x < baseX || y < baseY || x >= width || y >= height);
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

    void setCursor(int mode);

    Shape *getShape(int x, int y) {
        if (!isValid(x, y)) return NULL;
        return attr[x][y].topShape;
    }

    void write(int x, int y);

    void setShapePointer(Shape *s) {
        curShape = s;
    }

    Shape *read(int x, int y) {
        if (!isValid(x, y)) return NULL;
        return attr[x][y].topShape;
    }

    int readFillSeq(int x, int y);

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

    void setClearColor(int c = -1);

    void initColor() {
        setClearColor();
        setCurColor(Color::C_BLACK);
    }

    int setCurColor(int c);

    void init(int argc, char *argv[]);

    void display();

    void flush() {
        glutSwapBuffers();
        glFlush();
    }

    int getFillSeq() {
        return ++fillSeq;
    }

    void drawStrippleLine(int x1, int y1, int x2, int y2, bool isClear = false);
};

extern class window win;

#endif