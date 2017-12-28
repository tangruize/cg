#ifndef CG_MENU_H
#define CG_MENU_H

class Menu {
private:
    static int mainMenuId;
public:
    enum MENUS {
        Fill, CUT, CLEAR, UNDO
    };
    static const int NR_MENUS = 4;
    static const char *const strMenus[NR_MENUS];

    static int getMenuId() { return mainMenuId; }
    static void initMenu();
};

#endif