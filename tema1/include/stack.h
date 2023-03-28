#ifndef STACK
#define STACK

#include <stdlib.h>

struct Stack {
  struct SinglyNode *tail;
};

void pushToStack(struct Stack **stack, void *data, size_t dataSize);
void *popFromStack(struct Stack **stack);
void clearStack(struct Stack **stack);

#endif
