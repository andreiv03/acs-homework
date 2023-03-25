#include <stdlib.h>
#include <string.h>

#include "../include/list.h"
#include "../include/queue.h"

void pushToQueue(struct Queue **queue, void *data, size_t dataSize) {
  if ((*queue)->head == NULL) {
    pushSinglyNodeAtEnd(&(*queue)->head, data, dataSize);
    (*queue)->tail = (*queue)->head;
  } else {
    pushSinglyNodeAtEnd(&(*queue)->tail, data, dataSize);
    (*queue)->tail = (*queue)->tail->next;
  }
}

void *popFromQueue(struct Queue **queue) {
  if ((*queue)->head == NULL)
    return NULL;

  struct SinglyNode *node = (*queue)->head;
  void *data = node->data;

  (*queue)->head = (*queue)->head->next;
  if ((*queue)->head == NULL)
    (*queue)->tail = NULL;

  free(node);
  return data;
}
