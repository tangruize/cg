#include "shape.h"
#include "mouse.h"
#include "draw.h"
#include "menu.h"

#include <GL/glut.h>

static const char *menus[3] = {
        "LOAD", "SAVE", "CLEAR"
};

static void subMenuShape(GLint opt) {
    shape = opt;
    resetCnt();
}

static void subMenuColor(GLint opt) {
    color = opt;
}

static void clear() {
    resetCnt();
    clearAll();
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

static void mainMenu(GLint opt) {
    switch (opt) {
        case LOAD:
            clear();
            load();
            break;
        case SAVE:
            save();
            break;
        case CLEAR:
            clear();
            break;
    }
}

void initMenu() {
    GLint menu1 = glutCreateMenu(subMenuShape);
    {
        for (unsigned i = 0; i < sizeof(shapes) / sizeof(shapes[0]); ++i) {
            glutAddMenuEntry(shapes[i], i);
        }
    }
    GLint menu2 = glutCreateMenu(subMenuColor);
    {
        for (unsigned i = 0; i < sizeof(colors) / sizeof(colors[0]); ++i) {
            glutAddMenuEntry(colors[i], i);
        }
    }
    glutCreateMenu(mainMenu);
    {
        glutAddSubMenu("FIGURE", menu1);
        glutAddSubMenu("COLOR", menu2);
        for (unsigned i = 0; i < sizeof(menus) / sizeof(menus[0]); ++i) {
            glutAddMenuEntry(menus[i], i);
        }
    }
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}