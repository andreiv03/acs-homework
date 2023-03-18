#include <string.h>

#include "../include/list.h"

void pushToStack(struct DoublyNode** tail, int* data) {
	struct DoublyNode* node = createDoublyNode();
	node->data = calloc(1, sizeof(int));
	*(int*)node->data = *data;

	if (*tail == NULL)
		*tail = node;
	else
		(*tail)->next = node, node->prev = *tail, *tail = node;
}

struct DoublyNode* popFromStack(struct DoublyNode** tail) {
	if (*tail == NULL)
		return NULL;

	struct DoublyNode* node = createDoublyNode();
	node->data = calloc(1, sizeof(int));
	*(int*)node->data = *(int*)(*tail)->data;

	if ((*tail)->prev)
		*tail = (*tail)->prev;

	return node;
}

void clearStack(struct DoublyNode** tail) {
	struct DoublyNode* node = *tail;

	while (*tail) {
		*tail = (*tail)->prev;
		freeDoublyNode(node);
		node = *tail;
	}
}
