#include "../include/list.h"

#include <stdio.h>
#include <stdlib.h>

struct SinglyNode* createSinglyNode() {
	struct SinglyNode* node;
	node = calloc(1, sizeof(struct SinglyNode));
	return node;
}

struct DoublyNode* createDoublyNode() {
	struct DoublyNode* node;
	node = calloc(1, sizeof(struct DoublyNode));
	return node;
}

void freeSinglyNode(struct SinglyNode* node) {
	free(node->data);
	free(node);
}

void freeDoublyNode(struct DoublyNode* node) {
	free(node->data);
	free(node);
}

void showDoublyList(FILE* file, struct DoublyNode* head, int pointer) {
	int index = 0;

	while (head) {
		if (index == pointer)
			fprintf(file, "|%c|", *(char*)head->data);
		else
			fprintf(file, "%c", *(char*)head->data);

		index = index + 1;
		head = head->next;
	}

	fprintf(file, "\n");
}

void showDoublyListPointer(FILE* file, struct DoublyNode* head, int pointer) {
	int index = 0;

	while (head) {
		if (index == pointer) {
			fprintf(file, "%c\n", *(char*)head->data);
			return;
		}

		index = index + 1;
		head = head->next;
	}
}
