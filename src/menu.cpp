#include "shape.h"
#include "mouse.h"
#include "draw.h"
#include "menu.h"
#include "file.h"
#include "window.h"

static void shapeMenu(GLint opt) {
    if (Shape::getCurType() == Shape::S_ERASER || Shape::getCurType() == Shape::S_FILL)
        win.setCursor(0);
    Shape::setCurType(opt);
}

static void colorMenu(GLint opt) {
    win.setCurColor(opt);
}

static void thickMenu(GLint opt) {
    Shape::setCurThick(opt);
}

static void eraserMenu(GLint opt) {
    win.setCursor(1);
    if (opt == 0)
        Shape::setCurType(Shape::S_ERASER_TOTAL);
    else
        Shape::setCurType(Shape::S_ERASER);
}

static void fileMenu(GLint opt) {
    switch (opt) {
        case File::SAVE:
            file.save();
            break;
        case File::LOAD:
            file.load();
            break;
        default:
            break;
    }
}

static void editMenu(GLint opt) {
    switch (opt) {
        case Menu::Fill:
            win.setCursor(2);
            shapeMenu(Shape::S_FILL);
            break;
        case Menu::CUT:
            shapeMenu(Shape::S_CUT);
            break;
        case Menu::CLEAR:
            Shape::clearAll();
            break;
        case Menu::UNDO:
            mouseReset();
            Shape::pop();
            break;
        default:
            break;
    }
}

static void mainMenu(GLint opt) {
    win.display();
}

void Menu::initMenu() {
    GLint menu1 = glutCreateMenu(shapeMenu);
    for (unsigned i = 0; i < Shape::NR_SHAPES; ++i) {
        glutAddMenuEntry(Shape::strShapes[i], i);
    }
    GLint menu2 = glutCreateMenu(colorMenu);
    for (unsigned i = 0; i < Color::NR_COLORS; ++i) {
        glutAddMenuEntry(Color::strColors[i], i);
    }
    GLint menu3 = glutCreateMenu(thickMenu);
    for (unsigned i = 0; i < Shape::NR_THICKS; ++i) {
        glutAddMenuEntry(Shape::strThicks[i], i);
    }
    GLint menu4 = glutCreateMenu(fileMenu);
    for (unsigned i = 0; i < File::NR_FILE_OP; ++i) {
        glutAddMenuEntry(File::strFileOp[i], i);
    }
    GLint menu5 = glutCreateMenu(eraserMenu);
    for (unsigned i = 0; i < Shape::NR_ERASERS; ++i) {
        glutAddMenuEntry(Shape::strErasers[i], i);
    }
    GLint menu6 = glutCreateMenu(editMenu);
    for (unsigned i = 0; i < NR_MENUS; ++i) {
        glutAddMenuEntry(strMenus[i], i);
    }
    glutCreateMenu(mainMenu);
    glutAddSubMenu("Figure", menu1);
    glutAddSubMenu("Color", menu2);
    glutAddSubMenu("Thickness", menu3);
//    glutAddSubMenu("File", menu4);
    glutAddSubMenu("Eraser", menu5);
    glutAddSubMenu("Edit", menu6);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

const char *const Menu::strMenus[] = {
        "Fill", "Cut", "Clear", "Undo"
};