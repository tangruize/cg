#include <iostream>
#include "shape.h"
#include "mouse.h"
#include "draw.h"
#include "menu.h"
#include "file.h"
#include "window.h"

void shapeMenu(int opt) {
    win.setCursor(0);
    Shape::setCurType(opt);
    Shape::setCurve(opt);
    if (Shape::isCurve())
        Shape::setCurType(Shape::S_POLYGON);
    mouseReset();
}

void colorMenu(int opt) {
    win.setCurColor(opt);
}

void thickMenu(int opt) {
    Shape::setCurThick(opt);
}

void eraserMenu(int opt) {
    win.setCursor(1);
    if (opt == 0)
        Shape::setCurType(Shape::S_ERASER_TOTAL);
    else
        Shape::setCurType(Shape::S_ERASER);
    mouseReset();
}

void fileMenu(int opt) {
    switch (opt) {
        case File::SAVE:
            file.save();
            break;
        case File::LOAD:
            file.load();
            win.display();
            break;
        default:
            break;
    }
}

void editMenu(int opt) {
    switch (opt) {
        case Menu::FILL:
            shapeMenu(Shape::S_FILL);
            win.setCursor(2);
            break;
        case Menu::CUT:
            shapeMenu(Shape::S_CUT);
            break;
        case Menu::CLEAR:
            Shape::clearAll();
            win.display();
            break;
        case Menu::UNDO:
            mouseReset();
            Shape::pop();
            win.display();
            break;
        default:
            break;
    }
}

void transMenu(int opt) {
    Coordinate::setType(opt);
    Shape::setCurType(Shape::S_TRANS);
    mouseReset();
}

void mainMenu(int opt) {
    if (opt == 0) {
        if (win.isSubWinCreated())
            win.destroySubWin();
        else
            win.createSubWin();
    }
    win.display();
}

void Menu::initMenu() {
    int menu1 = glutCreateMenu(shapeMenu);
    for (unsigned i = 0; i < Shape::NR_SHAPES; ++i) {
        glutAddMenuEntry(Shape::strShapes[i], i);
    }
    int menu2 = glutCreateMenu(colorMenu);
    for (unsigned i = 0; i < Color::NR_COLORS; ++i) {
        glutAddMenuEntry(Color::strColors[i], i);
    }
    int menu3 = glutCreateMenu(thickMenu);
    for (unsigned i = 0; i < Shape::NR_THICKS; ++i) {
        glutAddMenuEntry(Shape::strThicks[i], i);
    }
    int menu4 = glutCreateMenu(fileMenu);
    for (unsigned i = 0; i < File::NR_FILE_OP; ++i) {
        glutAddMenuEntry(File::strFileOp[i], i);
    }
    int menu5 = glutCreateMenu(eraserMenu);
    for (unsigned i = 0; i < Shape::NR_ERASERS; ++i) {
        glutAddMenuEntry(Shape::strErasers[i], i);
    }
    int menu6 = glutCreateMenu(editMenu);
    for (unsigned i = 0; i < NR_MENUS; ++i) {
        glutAddMenuEntry(strMenus[i], i);
    }
    int menu7 = glutCreateMenu(transMenu);
    for (unsigned i = 0; i < Coordinate::NR_TRANS; ++i) {
        glutAddMenuEntry(Coordinate::strTrans[i], i);
    }
    mainMenuId = glutCreateMenu(mainMenu);
    glutAddSubMenu("Figure", menu1);
    glutAddSubMenu("Color", menu2);
    glutAddSubMenu("Thickness", menu3);
    glutAddSubMenu("File", menu4);
    glutAddSubMenu("Eraser", menu5);
    glutAddSubMenu("Edit", menu6);
    glutAddSubMenu("Transform", menu7);
    glutAddMenuEntry("3D", 0);
    glutSetMenu(mainMenuId);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

const char *const Menu::strMenus[] = {
        "Fill", "Cut", "Clear", "Undo"
};

int Menu::mainMenuId;