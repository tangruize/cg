#include "shape.h"
#include "mouse.h"
#include "window.h"

static int mouseActive = -1;
static bool mouseUp = false;

void mouseReset() {
    mouseActive = -1;
    mouseUp = false;
}

void mouseClickFunc(int button, int state, int x, int y) {
    Shape *s;
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseUp = false;
            switch (Shape::getCurType()) {
                case Shape::S_POINT:
                    s = new CurveShape(x, y);
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
                default:
                    break;
            }
            mouseActive = Shape::getCurType();
        } else {
            mouseUp = true;
            s = Shape::getLastShape();
            if (s) {
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
    if (!s || s->getType() != Shape::getCurType()) return;
    switch (Shape::getCurType()) {
        case Shape::S_ERASER:
            ((EraserShape *) s)->addVertex(x, y);
            s->drawLast();
            break;
        case Shape::S_POINT:
            ((CurveShape *) s)->addVertex(x, y);
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
        default:
            break;
    }
    win.flush();
}