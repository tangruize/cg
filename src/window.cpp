//
// Created by tangruize on 17-11-29.
//

#include "draw.h"
#include "window.h"
#include "menu.h"
#include "mouse.h"
#include "display.h"
#include "keyboard.h"

window win;

void window::init(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);

    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Computer Graphics Lab");

    glutDisplayFunc(displayFunc);
    glutReshapeFunc(reshapeFunc);
    glutMouseFunc(mouseClickFunc);
    glutMotionFunc(mouseMotionFunc);
    glutKeyboardFunc(keyboardFunc);

    Menu::initMenu();
    initColor();

    glutMainLoop();
}

int window::readFillSeq(int x, int y) {
    if (!isValid(x, y)) return 0;
    if (attr[x][y].topShape && attr[x][y].topShape->getType() != Shape::S_FILL)
        return 0;
    return attr[x][y].fillSeq;
}

void window::drawStrippleLine(int x1, int y1, int x2, int y2, bool isClear) {
    int preColor;
    if (isClear)
    {
        preColor = setCurColor(Color::C_WHITE);
        int thickness = Shape::getCurThick();
        Shape::setCurThick(Shape::T_SMALL);
        bool preTmpMode = tmpDrawFlag, preEraseMode = eraseFlag;
        tmpDrawFlag = eraseFlag = true;
        curShape = NULL;
        Draw::line(x1, y1, x2, y1);
        Draw::line(x2, y1, x2, y2);
        Draw::line(x2, y2, x1, y2);
        Draw::line(x1, y2, x1, y1);
        tmpDrawFlag = preTmpMode;
        eraseFlag = preEraseMode;
        Shape::setCurThick(thickness);
    }
    else
    {
        preColor = setCurColor(Color::C_BLACK);
        glLineStipple(2, 0x5555);
        glEnable(GL_LINE_STIPPLE);
        glBegin(GL_LINES);
        float fx1, fy1, fx2, fy2;
        Coordinate::coorTrans(x1, y1, fx1, fy1);
        Coordinate::coorTrans(x2, y2, fx2, fy2);
        glVertex2f(fx1, fy1);
        glVertex2f(fx2, fy1);
        glVertex2f(fx2, fy1);
        glVertex2f(fx2, fy2);
        glVertex2f(fx2, fy2);
        glVertex2f(fx1, fy2);
        glVertex2f(fx1, fy2);
        glVertex2f(fx1, fy1);
        glEnd();
        glDisable(GL_LINE_STIPPLE);
    }
    setCurColor(preColor);
}

bool window::getThickPoint(int &x, int &y, int seq, int thickness) {
    switch (thickness) {
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
            return false;
    }
}

void window::clear(int x, int y) {
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

window::window(int w, int h) : width(w), height(h) {
    createWindow();
}

void window::write(int x, int y) {
    if (!isValid(x, y)) return;
    int thickness = Shape::getCurThick();
    for (int i = 0; getThickPoint(x, y, i, thickness); ++i) {
        if (!isValid(x, y))
            continue;
        if (eraseFlag)
            clear(x, y);
        else {
            if (!tmpDrawFlag || attr[x][y].topShape == NULL) {
                attr[x][y].topShape = curShape;
                if (fillFlag)
                    attr[x][y].fillSeq = fillSeq;
                float fx, fy;
                Coordinate::coorTrans(x, y, fx, fy);
                glVertex2f(fx, fy);
            }
        }
    }
}

void window::setClearColor(int c) {
    float *color4fv = Color::setClearColor(c);
    if (color4fv)
        glClearColor(color4fv[0], color4fv[1], color4fv[2], color4fv[3]);
}

int window::setCurColor(int c) {
    int pre;
    float *color4fv = Color::setCurColor(c, pre);
    glColor4fv(color4fv);
    return pre;
}

void window::display() {
    glClear(GL_COLOR_BUFFER_BIT);
    Shape::drawAll();
    glutSwapBuffers();
    glFlush();
}

void window::setCursor(int mode) {
    if (mode == 1) {
        glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    } else if (mode == 0) {
        glutSetCursor(GLUT_CURSOR_INHERIT);
    } else if (mode == 2) {
        glutSetCursor(GLUT_CURSOR_SPRAY);
    }
}

void window::createWindow() {
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
