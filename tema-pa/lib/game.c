#include "../include/game.h"
#include "../include/list.h"
#include "../include/queue.h"
#include "../include/stack.h"
#include "../include/tree.h"
#include "../include/utils.h"
#include <stdio.h>
#include <string.h>

Game* readTeams(FILE* inputFileStream) {
	int teamsLength = 0;
	fscanf(inputFileStream, "%d", &teamsLength);

	Game* game = calloc(1, sizeof(Game));
	game->teamsLength = teamsLength;

	for (int index = 0; index < teamsLength; ++index) {
		int playersLength = 0;
		char teamName[256];
		fscanf(inputFileStream, "%d ", &playersLength);
		fscanf(inputFileStream, "%[^\n]%*c", teamName);

		if (teamName[strlen(teamName) - 2] == ' ')
			teamName[strlen(teamName) - 2] = '\0';
		else
			teamName[strlen(teamName) - 1] = '\0';

		Team* team = calloc(1, sizeof(Team));
		team->name = calloc(1, sizeof(char) * (strlen(teamName) + 1));
		strcpy(team->name, teamName);
		team->playersLength = playersLength;

		for (int index = 0; index < playersLength; ++index) {
			char firstName[256];
			char secondName[256];
			int points = 0;
			fscanf(inputFileStream, "%s %s %d", firstName, secondName, &points);

			Player* player = calloc(1, sizeof(Player));
			player->firstName = calloc(1, sizeof(char) * (strlen(firstName) + 1));
			player->secondName = calloc(1, sizeof(char) * (strlen(secondName) + 1));
			strcpy(player->firstName, firstName);
			strcpy(player->secondName, secondName);
			player->points = points;
			team->points = team->points + player->points;

			pushListNodeAtBeginning(&team->players, player, sizeof(Player));
		}

		team->points = team->points / team->playersLength;
		pushListNodeAtBeginning(&game->teams, team, sizeof(Team));
	}

	return game;
}

Game* filterTeams(FILE* inputFileStream, Game* game) {
	int highestPowerOfTwo = getHighestPowerOfTwo(game->teamsLength);

	for (int index = highestPowerOfTwo; index < game->teamsLength; ++index) {
		ListNode* head = game->teams;
		ListNode* previous = NULL;
		ListNode* current = head;

		while (head->next != NULL) {
			Team* team = head->next->data;

			if (team->points < ((Team*)current->data)->points) {
				previous = head;
				current = head->next;
			}

			head = head->next;
		}

		if (previous == NULL)
			game->teams = current->next;
		else
			previous->next = current->next;

		free(current);
	}

	game->teamsLength = highestPowerOfTwo;
	return game;
}

void printTeams(FILE* outputFileStream, Game* game) {
	ListNode* head = game->teams;
	while (head != NULL) {
		Team* team = head->data;
		fprintf(outputFileStream, "%s\n", team->name);
		head = head->next;
	}
}

void incrementTeamPoints(ListNode* head, char* teamName) {
	while (head != NULL) {
		Team* team = head->data;
		if (strcmp(team->name, teamName) == 0)
			team->points = team->points + 1;
		head = head->next;
	}
}

Game* startGame(FILE* outputFileStream, Game game) {
	Game* currentGame = &game;
	Game* topEightTeams = calloc(1, sizeof(Game));

	Queue* matches = calloc(1, sizeof(Queue));
	Stack* winners = calloc(1, sizeof(Stack));
	Stack* losers = calloc(1, sizeof(Stack));

	for (int index = 1; currentGame->teamsLength > 1; index++) {
		clearQueue(&matches);
		ListNode* head = currentGame->teams;
		fprintf(outputFileStream, "\n--- ROUND NO:%d\n", index);

		while (head != NULL) {
			Team* teamOne = head->data;
			Team* teamTwo = head->next->data;
			fprintf(outputFileStream, "%-33s-%33s\n", teamOne->name, teamTwo->name);

			pushToQueue(&matches, teamOne, sizeof(Team));
			pushToQueue(&matches, teamTwo, sizeof(Team));

			if (currentGame->teamsLength == 8) {
				pushListNodeAtBeginning(&topEightTeams->teams, teamOne, sizeof(Team));
				pushListNodeAtBeginning(&topEightTeams->teams, teamTwo, sizeof(Team));
				topEightTeams->teamsLength = topEightTeams->teamsLength + 2;
			}

			head = head->next->next;
		}

		head = matches->head;
		fprintf(outputFileStream, "\nWINNERS OF ROUND NO:%d\n", index);

		while (head != NULL) {
			Team* teamOne = (Team*)head->data;
			Team* teamTwo = (Team*)head->next->data;

			if (teamOne->points > teamTwo->points) {
				pushToStack(&winners, teamOne, sizeof(Team));
				pushToStack(&losers, teamTwo, sizeof(Team));
			} else {
				pushToStack(&winners, teamTwo, sizeof(Team));
				pushToStack(&losers, teamOne, sizeof(Team));
			}

			((Team*)winners->tail->data)->points = ((Team*)winners->tail->data)->points + 1;
			incrementTeamPoints(currentGame->teams, ((Team*)winners->tail->data)->name);

			head = head->next->next;
		}

		clearStack(&losers);
		currentGame->teams = NULL;

		while (winners->tail != NULL) {
			Team* winner = popFromStack(&winners);
			fprintf(outputFileStream, "%-33s -  %.2f\n", winner->name, winner->points);

			pushListNodeAtEnd(&currentGame->teams, winner, sizeof(Team));
		}

		currentGame->teamsLength = currentGame->teamsLength / 2;
		matches->head = NULL;
	}

	return topEightTeams;
}

void sortTopEightTeams(FILE* outputFileStream, Game* topEightTeams) {
	TreeNode* root = NULL;
	ListNode* head = topEightTeams->teams;

	while (head != NULL) {
		Team* team = head->data;
		root = insertTreeNodeInBST(root, team);
		head = head->next;
	}

	fprintf(outputFileStream, "\nTOP 8 TEAMS:\n");
	printBST(outputFileStream, root);
}

void printLevelTwoTeams(FILE* outputFileStream, Game* topEightTeams) {
	TreeNode* root = NULL;
	ListNode* head = topEightTeams->teams;

	while (head != NULL) {
		Team* team = head->data;
		root = insertTreeNodeInAVL(root, team);
		head = head->next;
	}

	fprintf(outputFileStream, "\nTHE LEVEL 2 TEAMS ARE:\n");
	int index = 0;
	printLevelTwoFromAVL(outputFileStream, root, &index);
}