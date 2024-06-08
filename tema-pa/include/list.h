#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>

typedef struct ListNode {
	void* data;
	struct ListNode* next;
} ListNode;

ListNode* createListNode();
void pushListNodeAtBeginning(ListNode** head, void* data, size_t dataSize);
void pushListNodeAtEnd(ListNode** head, void* data, size_t dataSize);
void freeListNode(ListNode* node);
void freeList(ListNode** head);

#endif
