#ifndef QUEUE
#define QUEUE

#include <stdlib.h>

struct Queue {
  struct SinglyNode *head;
  struct SinglyNode *tail;
};

void pushToQueue(struct Queue **queue, void *data, size_t dataSize);
void *popFromQueue(struct Queue **queue);

#endif
