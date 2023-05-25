#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "screen_functions.h"

int main() {
    initscr();
    noecho();
    TMenu menu;
    menu.difficulty = 1;
    char choice = '?';
    while ((choice != '1') && (choice != '3')) {
        PrintMenu(&menu, &choice);
    }
    
    endwin();
    return 0;
}