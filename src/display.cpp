#include "display.h"
#include "window.h"

void displayFunc() {
    win.display();
}

void reshapeFunc(int w,int h) {
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    win.destroyWindow();
    win.createWindow(w, h);
    win.display();
}