#ifndef _MAZE_H_
#define _MAZE_H_

typedef struct Coordinates {
	int x;
	int y;
} Coordinates;

typedef struct MazeCell {
	unsigned int data : 1;
} MazeCell;

typedef struct Maze {
	int height;
	int width;
	int difficulty;
	Coordinates* player;
	Coordinates* finish;
	MazeCell** cells;
} Maze;

Maze* createMaze();
void freeMaze(Maze* maze);
void readInput(Maze* maze);
void startGame(Maze* maze, char* choice);
void winGame(Maze* maze, char* choice);
void loseGame(Maze* maze, char* choice);

#endif
