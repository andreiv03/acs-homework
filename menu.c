#include "./menu.h"
#include "./maze.h"
#include <ncurses.h>

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

	refresh();
	*choice = getch();

	if (*choice == '2')
		menu->difficulty++;

	if (menu->difficulty == 4)
		menu->difficulty = 1;
}