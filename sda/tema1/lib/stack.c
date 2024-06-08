#include <stdlib.h>

#include "../include/list.h"
#include "../include/stack.h"

void pushToStack(struct Stack **stack, void *data, size_t dataSize) {
  pushSinglyNodeAtBeginning(&(*stack)->tail, data, dataSize);
}

void *popFromStack(struct Stack **stack) {
  if ((*stack)->tail == NULL)
    return NULL;

  struct SinglyNode *node = (*stack)->tail;
  void *data = (*stack)->tail->data;
  (*stack)->tail = (*stack)->tail->next;

  free(node);
  return data;
}

void clearStack(struct Stack **stack) {
  if ((*stack)->tail == NULL)
    return;

  struct SinglyNode *node = (*stack)->tail;

  while ((*stack)->tail != NULL) {
    (*stack)->tail = (*stack)->tail->next;
    freeSinglyNode(node);
    node = (*stack)->tail;
  }
}
