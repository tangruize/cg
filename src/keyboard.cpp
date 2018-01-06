//
// Created by tangruize on 17-11-28.
//

#include "keyboard.h"
#include "shape.h"
#include "display.h"
#include "window.h"
#include "mouse.h"
#include "menu.h"
#include "file.h"
#include "color.h"
#include "coordinate.h"

#include <cstdlib>

#define ALPHA_KEY(x) (x + 'A' - 1)

void keyboardFunc(unsigned char key, int x, int y) {
    int mod = glutGetModifiers();
    if (key == 27) {
        glutDestroyWindow(glutGetWindow());
        exit(0);
    } else if (mod & GLUT_ACTIVE_CTRL) {
        key = static_cast<unsigned char>(ALPHA_KEY(key));
        switch (key) {
            case 'S': fileMenu(File::SAVE); break;
            case 'L': fileMenu(File::LOAD); break;
            case 'F': editMenu(Menu::FILL); break;
            case 'C': editMenu(Menu::CUT); break;
            case 'P': editMenu(Menu::CLEAR); break;
            case 'U': editMenu(Menu::UNDO); break;
            case 'M': transMenu(Coordinate::T_MOVE); break;
            case 'R': transMenu(Coordinate::T_ROTATE); break;
            case 'Z': transMenu(Coordinate::T_ZOOM); break;
            case 'D': mainMenu(0); break;
        }
    } else if (key >= '0' && key <= '5') {
        shapeMenu(key - '0');
    } else {
        switch (key) {
            case 'w': colorMenu(Color::C_WHITE); break;
            case 'd': colorMenu(Color::C_BLACK); break;
            case 'r': colorMenu(Color::C_RED); break;
            case 'g': colorMenu(Color::C_GREEN); break;
            case 'b': colorMenu(Color::C_BLUE); break;
            case 'y': colorMenu(Color::C_YELLOW); break;
            case 'p': colorMenu(Color::C_PURPLE); break;
            case 'c': colorMenu(Color::C_CYAN); break;
            case 'o': colorMenu(Color::C_ORANGE); break;
            case 'S': thickMenu(Shape::T_SMALL); break;
            case 'M': thickMenu(Shape::T_MIDDLE); break;
            case 'B': thickMenu(Shape::T_BIG); break;
        }
    }
}