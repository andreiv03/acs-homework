#ifndef QUEUE
#define QUEUE

struct Queue {
  struct SinglyNode *head;
  struct SinglyNode *tail;
};

void pushToQueue(struct Queue **queue, void *data, size_t dataSize);
void *popFromQueue(struct Queue **queue);

#endif
