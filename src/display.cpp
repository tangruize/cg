#include "draw.h"
#include "display.h"

#include <GL/glut.h>

void myDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAllShapes();
    glFlush();
}
