#ifndef _MAZE_H_
#define _MAZE_H_

#include "./menu.h"
#include <stdio.h>

typedef struct Coordinates {
	int x;
	int y;
} Coordinates;

typedef struct MazeCell {
	int data;
} MazeCell;

typedef struct Maze {
	int height;
	int width;
	int difficulty;
	Coordinates* player;
	Coordinates* finish;
	MazeCell** cells;
} Maze;

void readInput(Maze* maze);
void startGame(Maze* maze, Menu* menu, char* choice);
void finishGame(Maze* maze);

#endif
