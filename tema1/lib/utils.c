#include "../include/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/list.h"

void getOperation(char* string, char* operation, char* value) {
	char* space = strchr(string, ' ');

	if (space == NULL) {
		strcpy(operation, string);
		strcpy(value, "");
		return;
	}

	strncpy(operation, string, space - string);
	operation[space - string] = '\0';

	*value = space[1];
}

int getOperationType(char* operation) {
	if (strcmp(operation, "MOVE_LEFT") == 0 ||
			strcmp(operation, "MOVE_RIGHT") == 0 ||
			strcmp(operation, "MOVE_LEFT_CHAR") == 0 ||
			strcmp(operation, "MOVE_RIGHT_CHAR") == 0 ||
			strcmp(operation, "WRITE") == 0 ||
			strcmp(operation, "INSERT_LEFT") == 0 ||
			strcmp(operation, "INSERT_RIGHT") == 0)
		return 1;

	if (strcmp(operation, "SHOW") == 0 ||
			strcmp(operation, "SHOW_CURRENT") == 0)
		return 2;

	if (strcmp(operation, "UNDO") == 0 ||
			strcmp(operation, "REDO") == 0)
		return 3;

	if (strcmp(operation, "EXECUTE") == 0)
		return 4;

	return 0;
}

void applyUpdateOperation(FILE* file, struct DoublyNode** head, int* pointer, char* operation, char* value) {
	struct DoublyNode* initial = *head;
	int index = 0;

	while (index != *pointer) {
		index = index + 1;
		*head = (*head)->next;
	}

	if (strcmp(operation, "MOVE_LEFT") == 0) {
		if (*pointer > 0)
			*pointer = *pointer - 1;
	}

	if (strcmp(operation, "MOVE_RIGHT") == 0) {
		if ((*head)->next == NULL) {
			struct DoublyNode* node = createDoublyNode();
			node->data = calloc(1, sizeof(char));
			*(char*)node->data = '#';
			node->prev = *head;
			(*head)->next = node;
			*head = node;
		}

		*pointer = *pointer + 1;
	}

	if (strcmp(operation, "MOVE_LEFT_CHAR") == 0) {
		int found = 0;
		int initialPointer = *pointer;

		while (*head) {
			if (*(char*)(*head)->data == *value) {
				found = 1;
				break;
			}

			*pointer = *pointer - 1;
			*head = (*head)->prev;
		}

		if (!found) {
			*pointer = initialPointer;
			fprintf(file, "%s\n", "ERROR");
		}
	}

	if (strcmp(operation, "MOVE_RIGHT_CHAR") == 0) {
		int found = 0;

		while ((*head)->next) {
			if (*(char*)(*head)->data == *value) {
				found = 1;
				break;
			}

			*pointer = *pointer + 1;
			*head = (*head)->next;
		}

		if (!found) {
			struct DoublyNode* node = createDoublyNode();
			node->data = calloc(1, sizeof(char));
			*(char*)node->data = '#';
			node->prev = *head;
			(*head)->next = node;
			*head = node;
			*pointer = *pointer + 1;
		}
	}

	if (strcmp(operation, "WRITE") == 0)
		*(char*)(*head)->data = *value;

	if (strcmp(operation, "INSERT_LEFT") == 0) {
		if (*pointer == 0)
			fprintf(file, "%s\n", "ERROR");
		else {
			struct DoublyNode* node = createDoublyNode();
			node->data = calloc(1, sizeof(char));
			*(char*)node->data = *value;
			node->next = *head;
			node->prev = (*head)->prev;
			(*head)->prev->next = node;
			(*head)->prev = node;
			*pointer = *pointer - 1;
		}
	}

	if (strcmp(operation, "INSERT_RIGHT") == 0) {
		struct DoublyNode* node = createDoublyNode();
		node->data = calloc(1, sizeof(char));
		*(char*)node->data = *value;
		node->prev = *head;

		if ((*head)->next) {
			node->next = (*head)->next;
			(*head)->next->prev = node;
		}

		(*head)->next = node;
		*pointer = *pointer + 1;
	}

	*head = initial;
}
