#include "../include/stack.h"
#include "../include/list.h"
#include <stdlib.h>

void pushToStack(Stack** stack, void* data, size_t dataSize) {
	pushListNodeAtBeginning(&(*stack)->tail, data, dataSize);
}

void* popFromStack(Stack** stack) {
	if ((*stack)->tail == NULL)
		return NULL;

	ListNode* node = (*stack)->tail;
	void* data = (*stack)->tail->data;
	(*stack)->tail = (*stack)->tail->next;

	free(node);
	return data;
}

void clearStack(Stack** stack) {
	if ((*stack)->tail == NULL)
		return;

	ListNode* node = (*stack)->tail;

	while ((*stack)->tail != NULL) {
		(*stack)->tail = (*stack)->tail->next;
		freeListNode(node);
		node = (*stack)->tail;
	}
}