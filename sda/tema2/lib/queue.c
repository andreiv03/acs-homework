#include "../include/queue.h"
#include "../include/quadtree.h"
#include <stdlib.h>

void pushToQueue(Queue* queue, QuadTree* tree) {
	QueueNode* node = calloc(1, sizeof(QueueNode));
	node->data = tree;

	if (queue->head == NULL) {
		queue->head = node;
		queue->tail = node;
	} else {
		queue->tail->next = node;
		queue->tail = node;
	}
}

void popFromQueue(Queue* queue) {
	QueueNode* node = queue->head;
	queue->head = queue->head->next;

	if (queue->head == NULL)
		queue->tail = NULL;

	free(node);
}
