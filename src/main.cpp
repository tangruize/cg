#include "window.h"
#include "menu.h"
#include "mouse.h"
#include "file.h"
#include "display.h"

#include <GL/glut.h>

int main(int argc, char *argv[]) {
    if (argc > 1) {
        file_name = argv[1];
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Computer Graphics Lab");
    glutDisplayFunc(&myDisplay);
    initMenu();
    glutMouseFunc(mouseClick);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glutMainLoop();

    return 0;
}