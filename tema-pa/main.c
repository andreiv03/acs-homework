#include "./include/game.h"
#include <stdio.h>

int main(int argc, char const* argv[]) {
	FILE* tasksFileStream = fopen(argv[1], "r");
	if (tasksFileStream == NULL)
		return 1;

	FILE* inputFileStream = fopen(argv[2], "r");
	if (inputFileStream == NULL)
		return 1;

	FILE* outputFileStream = fopen(argv[3], "w");
	if (outputFileStream == NULL)
		return 1;

	int tasks[4];
	for (int index = 0; index < 5; ++index)
		fscanf(tasksFileStream, "%d", &tasks[index]);

	Game* game = NULL;
	Game* topEightTeams = NULL;

	if (tasks[0] == 1)
		game = readTeams(inputFileStream);
	if (tasks[1] == 1)
		game = filterTeams(inputFileStream, game);
	printTeams(outputFileStream, game);
	if (tasks[2] == 1)
		topEightTeams = startGame(outputFileStream, *game);
	if (tasks[3] == 1)
		sortTopEightTeams(outputFileStream, topEightTeams);
	if (tasks[4] == 1)
		printLevelTwoTeams(outputFileStream, topEightTeams);

	free(game);
	free(topEightTeams);

	fclose(tasksFileStream);
	fclose(inputFileStream);
	fclose(outputFileStream);

	return 0;
}