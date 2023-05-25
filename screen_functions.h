#include <ncurses.h>

typedef struct menu {
    int difficulty;
}TMenu;

void PrintMenu(TMenu *menu, char *choice);
