#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "./list.h"

typedef struct Queue {
	ListNode* head;
	ListNode* tail;
} Queue;

void pushToQueue(Queue** queue, void* data, size_t dataSize);
void* popFromQueue(Queue** queue);
void clearQueue(Queue** queue);

#endif
