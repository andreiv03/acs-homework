#ifndef _MENU_H_
#define _MENU_H_

typedef struct Menu {
	int difficulty;
	int verdict;
} Menu;

Menu* createMenu();
void freeMenu(Menu* menu);
void printMenu(Menu* menu, char* choice);

#endif
