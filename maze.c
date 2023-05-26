#include "./maze.h"
#include "./menu.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readInput(Maze* maze) {
	FILE* inputFileStream;

	if (maze->difficulty == 1)
		inputFileStream = fopen("./input/easy.in", "r");

	if (maze->difficulty == 2)
		inputFileStream = fopen("./input/medium.in", "r");

	if (maze->difficulty == 3)
		inputFileStream = fopen("./input/hard.in", "r");

	fscanf(inputFileStream, "%d", &maze->width);
	fscanf(inputFileStream, "%d", &maze->height);

	maze->cells = calloc(maze->height, sizeof(MazeCell*));

	for (int rows = 0; rows < maze->height; ++rows) {
		maze->cells[rows] = calloc(maze->width, sizeof(MazeCell));

		for (int columns = 0; columns < maze->width; ++columns) {
			int cell = 0;
			fscanf(inputFileStream, "%d", &cell);

			if (cell == 0 || cell == 1)
				maze->cells[rows][columns].data = cell;

			if (cell == 2) {
				maze->player->x = rows;
				maze->player->y = columns;
				maze->cells[rows][columns].data = 0;
			}

			if (cell == 3) {
				maze->finish->x = rows;
				maze->finish->y = columns;
				maze->cells[rows][columns].data = 0;
			}
		}
	}

	fclose(inputFileStream);
}

void updatePlayerCoordinates(Maze* maze, char* choice) {
	if (*choice == 'w')
		if (maze->cells[maze->player->x - 1][maze->player->y].data == 0)
			maze->player->x = maze->player->x - 1;

	if (*choice == 'a')
		if (maze->cells[maze->player->x][maze->player->y - 1].data == 0)
			maze->player->y = maze->player->y - 1;

	if (*choice == 's')
		if (maze->cells[maze->player->x + 1][maze->player->y].data == 0)
			maze->player->x = maze->player->x + 1;

	if (*choice == 'd')
		if (maze->cells[maze->player->x][maze->player->y + 1].data == 0)
			maze->player->y = maze->player->y + 1;
}

void finishGame(Maze* maze) {
	erase();

	printw("================ MAZE GAME ================\n\n");
	printw("Congratulations! You finished the maze!\n");
	printw("Press 'q' to quit the game.\n");

	refresh();
}

void startGame(Maze* maze, Menu* menu, char* choice) {
	erase();

	printw("================ MAZE GAME ================\n\n");
	printw("Use the WASD keys to move the player.\n");
	printw("Press 'q' to quit the game.\n\n");

	if (maze->difficulty == 1)
		printw("Difficulty : Easy\n");

	if (maze->difficulty == 2)
		printw("Difficulty : Medium\n");

	if (maze->difficulty == 3)
		printw("Difficulty : Hard\n\n");

	for (int rows = 0; rows < maze->height; ++rows) {
		for (int columns = 0; columns < maze->width; ++columns) {
			if (rows == maze->player->x && columns == maze->player->y)
				printw("P");

			else if (rows == maze->finish->x && columns == maze->finish->y)
				printw("F");

			else {
				if (maze->cells[rows][columns].data == 0)
					printw(" ");

				else if (maze->cells[rows][columns].data == 1)
					printw("#");
			}
		}

		printw("\n");
	}

	refresh();
	*choice = getch();

	updatePlayerCoordinates(maze, choice);
}