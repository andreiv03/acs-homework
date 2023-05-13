#include "../include/priority-queue.h"
#include <stdlib.h>

PriorityQueue* createPriorityQueue(int capacity) {
	PriorityQueue* priorityQueue = calloc(1, sizeof(PriorityQueue));
	priorityQueue->size = 0;
	priorityQueue->capacity = capacity;
	priorityQueue->nodes = calloc(capacity, sizeof(PriorityQueueNode));
	return priorityQueue;
}

int isPriorityQueueEmpty(PriorityQueue* priorityQueue) {
	return priorityQueue->size == 0;
}

void swapPriorityQueueNodes(PriorityQueueNode* a, PriorityQueueNode* b) {
	PriorityQueueNode temporaryNode = *a;
	*a = *b;
	*b = temporaryNode;
}

void heapifyUpPriorityQueue(PriorityQueue* priorityQueue, int index) {
	int parentIndex = (index - 1) / 2;

	PriorityQueueNode currentNode = priorityQueue->nodes[index];
	PriorityQueueNode parentNode = priorityQueue->nodes[parentIndex];

	if (index > 0 && currentNode.priority < parentNode.priority) {
		swapPriorityQueueNodes(&priorityQueue->nodes[index],
													 &priorityQueue->nodes[parentIndex]);
		heapifyUpPriorityQueue(priorityQueue, parentIndex);
	}
}

void heapifyDownPriorityQueue(PriorityQueue* priorityQueue, int index) {
	int leftChildIndex = 2 * index + 1;
	int rightChildIndex = 2 * index + 2;
	int smallestNodeIndex = index;

	PriorityQueueNode* leftChild = &priorityQueue->nodes[leftChildIndex];
	PriorityQueueNode* rightChild = &priorityQueue->nodes[rightChildIndex];
	PriorityQueueNode* smallestNode = &priorityQueue->nodes[smallestNodeIndex];

	if (leftChildIndex < priorityQueue->size)
		if (leftChild->priority < smallestNode->priority)
			smallestNodeIndex = leftChildIndex;

	if (rightChildIndex < priorityQueue->size)
		if (rightChild->priority < smallestNode->priority)
			smallestNodeIndex = rightChildIndex;

	if (smallestNodeIndex != index) {
		swapPriorityQueueNodes(&priorityQueue->nodes[index],
													 &priorityQueue->nodes[smallestNodeIndex]);
		heapifyDownPriorityQueue(priorityQueue, smallestNodeIndex);
	}
}

void pushToPriorityQueue(PriorityQueue* priorityQueue, int vertex,
												 int priority) {
	if (priorityQueue->size == priorityQueue->capacity)
		return;

	PriorityQueueNode node;
	node.vertex = vertex;
	node.priority = priority;

	priorityQueue->nodes[priorityQueue->size] = node;
	heapifyUpPriorityQueue(priorityQueue, priorityQueue->size);
	priorityQueue->size = priorityQueue->size + 1;
}

int popFromPriorityQueue(PriorityQueue* priorityQueue) {
	if (isPriorityQueueEmpty(priorityQueue))
		return -1;

	int vertex = priorityQueue->nodes[0].vertex;
	priorityQueue->size = priorityQueue->size - 1;
	priorityQueue->nodes[0] = priorityQueue->nodes[priorityQueue->size];
	heapifyDownPriorityQueue(priorityQueue, 0);

	return vertex;
}

void freePriorityQueue(PriorityQueue* priorityQueue) {
	free(priorityQueue->nodes);
	free(priorityQueue);
}