#ifndef LIST
#define LIST

#include <stdio.h>
#include <stdlib.h>

struct SinglyNode {
	void* data;
	struct SinglyNode* next;
};

struct DoublyNode {
	void* data;
	struct DoublyNode* next;
	struct DoublyNode* prev;
};

struct SinglyNode* createSinglyNode();
struct DoublyNode* createDoublyNode();

void freeSinglyNode(struct SinglyNode* node);
void freeDoublyNode(struct DoublyNode* node);

void showDoublyList(FILE* file, struct DoublyNode* head, int pointer);
void showDoublyListPointer(FILE* file, struct DoublyNode* head, int pointer);

#endif
