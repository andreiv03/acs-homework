#include "../include/queue.h"
#include "../include/list.h"
#include <stdlib.h>

void pushToQueue(Queue** queue, void* data, size_t dataSize) {
	if ((*queue)->head == NULL) {
		pushListNodeAtEnd(&(*queue)->head, data, dataSize);
		(*queue)->tail = (*queue)->head;
	} else {
		pushListNodeAtEnd(&(*queue)->tail, data, dataSize);
		(*queue)->tail = (*queue)->tail->next;
	}
}

void* popFromQueue(Queue** queue) {
	if ((*queue)->head == NULL)
		return NULL;

	ListNode* node = (*queue)->head;
	void* data = node->data;

	(*queue)->head = (*queue)->head->next;
	if ((*queue)->head == NULL)
		(*queue)->tail = NULL;

	free(node);
	return data;
}

void clearQueue(Queue** queue) {
	if ((*queue)->head == NULL)
		return;

	ListNode* node = (*queue)->head;

	while ((*queue)->head != NULL) {
		(*queue)->head = (*queue)->head->next;
		freeListNode(node);
		node = (*queue)->head;
	}

	(*queue)->tail = NULL;
}