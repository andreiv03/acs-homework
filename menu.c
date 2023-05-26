#include "./menu.h"
#include "./maze.h"
#include <ncurses.h>
#include <stdlib.h>

Menu* createMenu() {
	Menu* menu = calloc(1, sizeof(Menu));
	if (menu == NULL)
		exit(1);

	menu->difficulty = 1;
	return menu;
}

void freeMenu(Menu* menu) {
	free(menu);
}

void printMenu(Menu* menu, char* choice) {
	erase();
	printw("================ MAZE GAME ================\n\n");

	switch (menu->difficulty) {
	case 1:
		printw("Difficulty : Easy\n\n");
		break;

	case 2:
		printw("Difficulty : Medium\n\n");
		break;

	case 3:
		printw("Difficulty : Hard\n\n");
		break;

	default:
		break;
	}

	printw("Press '1' to START GAME\n");
	printw("Press '2' to CHANGE DIFFICULTY\n");
	printw("Press '3' to EXIT GAME\n");

	*choice = getch();

	if (*choice == '2') {
		menu->difficulty++;

		if (menu->difficulty == 4)
			menu->difficulty = 1;
	}

	refresh();
}