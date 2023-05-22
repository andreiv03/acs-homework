#include "../include/list.h"
#include <stdlib.h>
#include <string.h>

ListNode* createListNode() {
	ListNode* node = calloc(1, sizeof(ListNode));
	return node;
}

void pushListNodeAtBeginning(ListNode** head, void* data, size_t dataSize) {
	ListNode* node = createListNode();
	node->data = calloc(1, dataSize);
	memcpy(node->data, data, dataSize);

	if (*head == NULL) {
		*head = node;
		return;
	}

	node->next = *head;
	*head = node;
}

void pushListNodeAtEnd(ListNode** head, void* data, size_t dataSize) {
	ListNode* node = createListNode();
	node->data = calloc(1, dataSize);
	memcpy(node->data, data, dataSize);

	if (*head == NULL) {
		*head = node;
		return;
	}

	ListNode* temp = *head;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = node;
}

void freeListNode(ListNode* node) {
	free(node->data);
	free(node);
}

void freeList(ListNode** head) {
	while (*head != NULL) {
		ListNode* node = *head;
		*head = (*head)->next;
		freeListNode(node);
	}
}
