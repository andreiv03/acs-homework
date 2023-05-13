#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include "./graph.h"

typedef struct PriorityQueueNode {
	int vertex;
	int priority;
} PriorityQueueNode;

typedef struct PriorityQueue {
	int size;
	int capacity;
	PriorityQueueNode* nodes;
} PriorityQueue;

PriorityQueue* createPriorityQueue(int capacity);
int isPriorityQueueEmpty(PriorityQueue* priorityQueue);
void swapPriorityQueueNodes(PriorityQueueNode* a, PriorityQueueNode* b);
void heapifyUpPriorityQueue(PriorityQueue* priorityQueue, int index);
void heapifyDownPriorityQueue(PriorityQueue* priorityQueue, int index);
void pushToPriorityQueue(PriorityQueue* priorityQueue, int vertex,
												 int priority);
int popFromPriorityQueue(PriorityQueue* priorityQueue);
void freePriorityQueue(PriorityQueue* priorityQueue);

#endif
