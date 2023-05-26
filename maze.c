#include "./include/maze.h"
#include "./include/menu.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define PLAYER_SYMBOL "P"
#define FINISH_SYMBOL "F"
#define EMPTY_SYMBOL " "
#define WALL_SYMBOL "#"

#define LOWER_W_KEY 'w'
#define UPPER_W_KEY 'W'
#define LOWER_A_KEY 'a'
#define UPPER_A_KEY 'A'
#define LOWER_S_KEY 's'
#define UPPER_S_KEY 'S'
#define LOWER_D_KEY 'd'
#define UPPER_D_KEY 'D'

Maze* createMaze() {
	Maze* maze = calloc(1, sizeof(Maze));
	if (maze == NULL)
		exit(1);

	maze->player = calloc(1, sizeof(Coordinates));
	if (maze->player == NULL)
		exit(1);

	maze->finish = calloc(1, sizeof(Coordinates));
	if (maze->finish == NULL)
		exit(1);

	return maze;
}

void freeMaze(Maze* maze) {
	for (int rows = 0; rows < maze->height; ++rows)
		free(maze->cells[rows]);
	free(maze->player);
	free(maze->finish);
	free(maze);
}

void readInput(Maze* maze) {
	FILE* inputFileStream = NULL;

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
			unsigned int cell = 0;
			fscanf(inputFileStream, "%u", &cell);

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
	int x = maze->player->x;
	int y = maze->player->y;

	if (*choice == LOWER_W_KEY || *choice == UPPER_W_KEY)
		if (maze->cells[x - 1][y].data == 0)
			maze->player->x = x - 1;

	if (*choice == LOWER_A_KEY || *choice == UPPER_A_KEY)
		if (maze->cells[x][y - 1].data == 0)
			maze->player->y = y - 1;

	if (*choice == LOWER_S_KEY || *choice == UPPER_S_KEY)
		if (maze->cells[x + 1][y].data == 0)
			maze->player->x = x + 1;

	if (*choice == LOWER_D_KEY || *choice == UPPER_D_KEY)
		if (maze->cells[x][y + 1].data == 0)
			maze->player->y = y + 1;
}

void startGame(Maze* maze, char* choice) {
	erase();

	printw("================ MAZE GAME ================\n\n");
	printw("Use the WASD keys to move the player.\n");
	printw("Press 'q' to quit the game.\n\n");

	if (maze->difficulty == 1)
		printw("Difficulty : Easy\n\n");

	if (maze->difficulty == 2)
		printw("Difficulty : Medium\n\n");

	if (maze->difficulty == 3)
		printw("Difficulty : Hard\n\n");

	for (int rows = 0; rows < maze->height; ++rows) {
		for (int columns = 0; columns < maze->width; ++columns) {
			if (rows == maze->player->x && columns == maze->player->y)
				printw(PLAYER_SYMBOL);

			else if (rows == maze->finish->x && columns == maze->finish->y)
				printw(FINISH_SYMBOL);

			else {
				if (maze->cells[rows][columns].data == 0)
					printw(EMPTY_SYMBOL);

				else if (maze->cells[rows][columns].data == 1)
					printw(WALL_SYMBOL);
			}
		}

		printw("\n");
	}

	*choice = getch();
	updatePlayerCoordinates(maze, choice);

	refresh();
}

void winGame(Maze* maze, char* choice) {
	erase();

	printw("================ MAZE GAME ================\n\n");
	printw("Congratulations! You finished the maze!\n");
	printw("Press any key to quit the game.\n");

	*choice = getch();
	refresh();
}

void loseGame(Maze* maze, char* choice) {
	erase();

	printw("================ MAZE GAME ================\n\n");
	printw("You lost! Better luck next time!\n");
	printw("Press any key to quit the game.\n");

	*choice = getch();
	refresh();
}