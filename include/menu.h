#ifndef CG_MENU_H
#define CG_MENU_H

class Menu {
private:
    static int mainMenuId;
public:
    enum MENUS {
        FILL, CUT, CLEAR, UNDO
    };
    static const int NR_MENUS = 4;
    static const char *const strMenus[NR_MENUS];

    static int getMenuId() { return mainMenuId; }
    static void initMenu();
};


void shapeMenu(int opt);
void colorMenu(int opt);
void thickMenu(int opt);
void eraserMenu(int opt);
void fileMenu(int opt);
void editMenu(int opt);
void transMenu(int opt);
void mainMenu(int opt);

#endif