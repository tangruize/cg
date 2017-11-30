//
// Created by tangruize on 17-11-29.
//

#include "window.h"
#include "menu.h"
#include "mouse.h"
#include "display.h"
#include "keyboard.h"

window win;

void window::init(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Computer Graphics Lab");

    glutDisplayFunc(displayFunc);
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
