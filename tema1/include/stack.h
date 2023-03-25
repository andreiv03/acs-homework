#ifndef STACK
#define STACK

struct Stack {
  struct DoublyNode *tail;
};

void pushToStack(struct Stack **stack, void *data, size_t dataSize);
void *popFromStack(struct Stack **stack);
void clearStack(struct Stack **stack);

#endif
