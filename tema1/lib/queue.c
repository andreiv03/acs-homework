#include <string.h>

#include "../include/list.h"

void pushToQueue(struct SinglyNode** head, struct SinglyNode** tail, char* data) {
	struct SinglyNode* node = createSinglyNode();
	node->data = calloc(strlen(data) + 1, sizeof(char));
	strcpy(node->data, data);

	if (*head == NULL && *tail == NULL)
		*head = *tail = node;
	else
		(*tail)->next = node, *tail = node;
}

struct SinglyNode* popFromQueue(struct SinglyNode** head) {
	if (*head == NULL)
		return NULL;

	struct SinglyNode* node = createSinglyNode();
	node->data = calloc(strlen((*head)->data) + 1, sizeof(char));
	strcpy(node->data, (*head)->data);

	*head = (*head)->next;
	return node;
}
