#ifndef _STACK_H_
#define _STACK_H_

#include "./list.h"

typedef struct Stack {
	ListNode* tail;
} Stack;

void pushToStack(Stack** stack, void* data, size_t dataSize);
void* popFromStack(Stack** stack);
void clearStack(Stack** stack);

#endif
