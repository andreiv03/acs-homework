#include <ncurses.h>
#include "screen_functions.h"

void PrintMenu(TMenu *menu, char *choice) {
    erase();
    printw("================= MAZE GAME =================\n\n");
    switch(menu->difficulty) {
        case 1: {
            printw("\t    Difficulty : Easy \n\n");
            break;
        }
        case 2: {
            printw("\t    Difficulty : Normal \n\n");
            break;
        }
        case 3: {
            printw("\t    Difficulty : Hard \n\n");
            break;
        }
        default: {
            break;
        }
    }
    printw("Press \"1\" to   Start Game\n");
    printw("Press \"2\" to   Change The Difficulty\n");
    printw("Press \"3\" to   Exit\n");

    refresh();
    *choice = getch();
    if (*choice == '2') {
        menu->difficulty ++;
    }
    if (menu->difficulty == 4) {
        menu->difficulty = 1;
    }
}