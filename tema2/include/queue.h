#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "./quadtree.h"

typedef struct QueueNode {
	QuadTree* data;
	struct QueueNode* next;
} QueueNode;

typedef struct Queue {
	QueueNode* head;
	QueueNode* tail;
} Queue;

void pushToQueue(Queue* queue, QuadTree* tree);
void popFromQueue(Queue* queue);

#endif
