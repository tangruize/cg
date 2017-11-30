//
// Created by tangruize on 17-11-28.
//

#include "keyboard.h"
#include "shape.h"
#include "display.h"
#include "window.h"
#include "mouse.h"

void keyboardFunc(unsigned char key, int x, int y) {
    int mod = glutGetModifiers();
    if (key == 27) {
        glutDestroyWindow(glutGetWindow());
    } else if (key == 26 && (mod & GLUT_ACTIVE_CTRL)) {
        mouseReset();
        Shape::pop();
        win.display();
    }
}