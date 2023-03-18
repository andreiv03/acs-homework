#ifndef STACK
#define STACK

#include "./list.h"

void pushToStack(struct DoublyNode** tail, int* data);
struct DoublyNode* popFromStack(struct DoublyNode** tail);
void clearStack(struct DoublyNode** tail);

#endif