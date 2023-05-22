#ifndef _GAME_H_
#define _GAME_H_

#include "./list.h"
#include <stdio.h>

typedef struct Player {
	char* firstName;
	char* secondName;
	int points;
} Player;

typedef struct Team {
	char* name;
	float points;
	int playersLength;
	ListNode* players;
} Team;

typedef struct Game {
	int teamsLength;
	ListNode* teams;
} Game;

Game* readTeams(FILE* inputFileStream);
Game* filterTeams(FILE* inputFileStream, Game* game);
void printTeams(FILE* outputFileStream, Game* game);
void incrementTeamPoints(ListNode* head, char* teamName);
Game* startGame(FILE* outputFileStream, Game game);
void sortTopEightTeams(FILE* outputFileStream, Game* topEightTeams);
void printLevelTwoTeams(FILE* outputFileStream, Game* topEightTeams);

#endif
