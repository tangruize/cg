#include "display.h"
#include "window.h"

void displayFunc() {
    win.display();
}

void reshapeFunc(int w,int h) {
    win.reshape(w, h);
}

void subWinDisplayFunc() {
    win.draw3DHexahedron();
}