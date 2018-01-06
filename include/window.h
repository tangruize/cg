#ifndef CG_WINDOW_H
#define CG_WINDOW_H

#include "color.h"
#include "shape.h"
#include "coordinate.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glut32.lib")
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#include <GL/glut.h>

const int WIDTH = 1024;
const int HEIGHT = 576;
const int SUB_WIDTH = 200;
const int SUB_HEIGHT = 200;


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
    int mainWindow, subWindow;
    float halfWidth, halfHeight, rtri;
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

    void createSubWin();

    void reshape(int w, int h);

    void createWindow(int w, int h) {
        width = w;
        height = h;
        createWindow();
    }
    bool isSubWinCreated() {
        return subWindow != -1;
    }

    window() : window(WIDTH, HEIGHT) { subWindow = -1;}

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
    void draw3DHexahedron();

    void destroySubWin();
};

extern class window win;

#endif