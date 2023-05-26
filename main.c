#include "./maze.h"
#include "./menu.h"
#include <ncurses.h>

int main() {
	Maze* maze = createMaze();
	Menu* menu = createMenu();

	initscr();
	noecho();

	char choice = '?';
	while (choice != '1' && choice != '3')
		printMenu(menu, &choice);

	if (choice == '1') {
		maze->difficulty = menu->difficulty;
		readInput(maze);

		while (choice != 'q') {
			if (maze->player->x == maze->finish->x && maze->player->y == maze->finish->y) {
				menu->verdict = 1;
				break;
			}

			startGame(maze, &choice);
		}
	}

	if (menu->verdict == 1)
		winGame(maze, &choice);
	else if (menu->verdict == 0)
		loseGame(maze, &choice);

	endwin();

	freeMenu(menu);
	freeMaze(maze);

	return 0;
}