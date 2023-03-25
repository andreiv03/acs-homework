#include <stdlib.h>

#include "../include/list.h"
#include "../include/stack.h"

void pushToStack(struct Stack **stack, void *data, size_t dataSize) {
  pushDoublyNodeAtEnd(&(*stack)->tail, data, dataSize);
  if ((*stack)->tail->next)
    (*stack)->tail = (*stack)->tail->next;
}

void *popFromStack(struct Stack **stack) {
  if ((*stack)->tail == NULL)
    return NULL;

  struct DoublyNode *node = (*stack)->tail;
  void *data = (*stack)->tail->data;
  (*stack)->tail = (*stack)->tail->prev;

  free(node);
  return data;
}

void clearStack(struct Stack **stack) {
  struct DoublyNode *node = (*stack)->tail;

  while ((*stack)->tail) {
    (*stack)->tail = (*stack)->tail->next;
    freeDoublyNode(node);
    node = (*stack)->tail;
  }
}
