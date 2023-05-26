#ifndef _MENU_H_
#define _MENU_H_

typedef struct Menu {
	int difficulty;
} Menu;

void printMenu(Menu* menu, char* choice);

#endif
