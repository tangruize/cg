#include "shape.h"
#include "mouse.h"
#include "window.h"
#include "menu.h"

#include <vector>
#include <utility>

using namespace std;

static int mouseActive = -1;
static bool mouseUp = false;

static int srcSet = 0;
static Shape *transSelectShape = NULL;
static vector<pair<int, int>> transPoints;

void mouseReset() {
    mouseActive = -1;
    mouseUp = false;
    transPoints.clear();
    transSelectShape = NULL;
    srcSet = 0;
}

void mouseDownTrans(int x, int y) {
    switch (Coordinate::type) {
        case Coordinate::T_MOVE:
            Coordinate::setSrc(x, y);
            Coordinate::setSrc2(x, y);
            srcSet = 2;
            break;
        case Coordinate::T_ROTATE:
        case Coordinate::T_ZOOM:
            if (srcSet == 0) {
                Coordinate::setSrc(x, y);
                srcSet = 1;
            } else if (srcSet == 1) {
                Coordinate::setSrc2(x, y);
                srcSet = 2;
            } else
                srcSet = 0;
            break;
        default:
            srcSet = 0;
            transSelectShape = NULL;
            return;
    }
    if (srcSet == 2) {
        transSelectShape = win.getShape(x, y);
        if (transSelectShape == NULL) {
            srcSet = 1;
            Coordinate::setSrc(x, y);
            transPoints.clear();
        }
    }
}

void mouseMoveTrans(int x, int y) {
    if (srcSet != 2) return;
    if (!transSelectShape) return;
    transSelectShape->clear();
    Coordinate::setDest(x, y);
    int px, py;
    if (transPoints.empty()) {
        for (int i = 0; transSelectShape->getVertex(i, px, py); ++i)
            transPoints.emplace_back(px, py);
    }
    for (int i = 0; i < (int) transPoints.size(); ++i) {
        auto &t = transPoints[i];
        px = t.first;
        py = t.second;
        Coordinate::getTransResult(px, py);
        transSelectShape->setVertex(i, px, py);
    }
    if (Coordinate::type == Coordinate::T_MOVE) {
        Coordinate::setSrc(x, y);
        transPoints.clear();
    }
    win.toggleTmpMode();
    transSelectShape->draw();
    win.toggleTmpMode();
}

void mouseClickFunc(int button, int state, int x, int y) {
    Shape *s;
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseUp = false;
            switch (Shape::getCurType()) {
                case Shape::S_POINT:
                    s = new PencilShape(x, y);
                    Shape::push(s);
                    s->draw();
                    break;
                case Shape::S_ERASER:
                    s = new EraserShape(x, y);
                    Shape::push(s);
                    s->draw();
                    break;
                case Shape::S_ERASER_TOTAL:
                    s = new EraserTotalShape();
                    Shape::push(s);
                    if (((EraserTotalShape *) s)->addErasePos(x, y)) {
                        s->draw();
                        win.display();
                    }
                    break;
                case Shape::S_THREAD:
                    s = new ThreadShape(x, y);
                    Shape::push(s);
                    s->draw();
                    break;
                case Shape::S_POLYGON:
                    if (mouseActive == -1) {
                        if (Shape::isCurve())
                            s = new CurveShape();
                        else
                            s = new PolygonShape();
                        ((PolygonShape *) s)->addVertex(x, y);
                        Shape::push(s);
                        glutDetachMenu(GLUT_RIGHT_BUTTON);
                    } else if (mouseActive == Shape::S_POLYGON) {
                        s = Shape::getLastShape();
                        if (s) {
                            ((PolygonShape *) s)->addVertex(x, y);
                            s->drawLast();
                            win.display();
                        }
                    }
                    break;
                case Shape::S_CIRCLE:
                    s = new CircleShape(x, y);
                    Shape::push(s);
                    break;
                case Shape::S_ELLIPSE:
                    s = new EllipseShape(x, y);
                    Shape::push(s);
                    break;
                case Shape::S_FILL:
                    s = new FillShape(x, y);
                    Shape::push(s);
                    s->draw();
                    break;
                case Shape::S_CUT:
                    if (mouseActive == -1) {
                        s = new CutShape(x, y);
                        Shape::push(s);
                    } else if (mouseActive == Shape::S_CUT) {
                        s = Shape::getLastShape();
                        if (s && s->getType() == Shape::S_CUT) {
                            ((CutShape *) s)->updateVertex2(x, y);
                            s->draw();
                            win.display();
                        }
                        mouseActive = -1;
                    }
                    break;
                case Shape::S_TRANS:
                    mouseDownTrans(x, y);
                    break;
                default:
                    break;
            }
            mouseActive = Shape::getCurType();
        } else {
            mouseUp = true;
            s = Shape::getLastShape();
            if (s) {
                if (mouseActive == Shape::S_TRANS && srcSet == 2) {
                    win.display();
                    srcSet = 0;
                }
                if (mouseActive != Shape::S_POLYGON && mouseActive != Shape::S_CUT)
                    mouseActive = -1;
            }
        }
    } else {
        if (mouseActive == Shape::S_POLYGON) {
            s = Shape::getLastShape();
            if (s) {
                ((PolygonShape *) s)->setComplete();
                s->draw();
            }
            glutSetMenu(Menu::getMenuId());
            glutAttachMenu(GLUT_RIGHT_BUTTON);
        }
        mouseActive = -1;
    }
    win.flush();
}

void mouseMotionFunc(int x, int y) {
    if (mouseActive == -1)
        return;
    Shape *s = Shape::getLastShape();
    if (Shape::getCurType() != Shape::S_TRANS
        && (!s || s->getType() != Shape::getCurType()))
        return;
    switch (Shape::getCurType()) {
        case Shape::S_ERASER:
            ((EraserShape *) s)->addVertex(x, y);
            s->drawLast();
            break;
        case Shape::S_POINT:
            ((PencilShape *) s)->addVertex(x, y);
            s->drawLast();
            break;
        case Shape::S_ERASER_TOTAL:
            if (((EraserTotalShape *) s)->addErasePos(x, y)) {
                s->drawLast();
                win.display();
            }
            break;
        case Shape::S_THREAD:
            ((ThreadShape *) s)->updateVertex2(x, y);
            s->drawLast();
            win.display();
            break;
        case Shape::S_POLYGON:
            if (mouseUp)
                ((PolygonShape *) s)->addVertex(x, y);
            else
                ((PolygonShape *) s)->updateLastVertex(x, y);
            s->drawLast();
            win.display();
            break;
        case Shape::S_CIRCLE:
            ((CircleShape *) s)->updateR(x, y);
            s->drawLast();
            win.display();
            break;
        case Shape::S_ELLIPSE:
            ((EllipseShape *) s)->updateVertex2(x, y);
            s->drawLast();
            win.display();
            break;
        case Shape::S_TRANS:
            mouseMoveTrans(x, y);
            break;
        default:
            break;
    }
    win.flush();
}