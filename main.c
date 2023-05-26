#include "./maze.h"
#include "./menu.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	Maze* maze = calloc(1, sizeof(Maze));
	if (maze == NULL)
		exit(1);

	maze->player = calloc(1, sizeof(Coordinates));
	if (maze->player == NULL)
		exit(1);

	maze->finish = calloc(1, sizeof(Coordinates));
	if (maze->finish == NULL)
		exit(1);

	Menu* menu = calloc(1, sizeof(Menu));
	menu->difficulty = 1;

	initscr();
	noecho();

	char choice = '?';
	while (choice != '1' && choice != '3')
		printMenu(menu, &choice);

	if (choice == '1') {
		maze->difficulty = menu->difficulty;
		readInput(maze);

		while (choice != 'q') {
			startGame(maze, menu, &choice);

			if (maze->player->x == maze->finish->x && maze->player->y == maze->finish->y)
				finishGame(maze);
		}
	}

	endwin();

	free(menu);

	free(maze->player);
	free(maze->finish);
	for (int rows = 0; rows < maze->height; ++rows)
		free(maze->cells[rows]);
	free(maze->cells);
	free(maze);

	return 0;
}