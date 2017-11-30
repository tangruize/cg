#ifndef CG_MENU_H
#define CG_MENU_H

class Menu {
public:
    enum MENUS {
        Fill, CUT, CLEAR, UNDO
    };
    static const int NR_MENUS = 4;
    static const char *const strMenus[NR_MENUS];

    static void initMenu();
};

#endif