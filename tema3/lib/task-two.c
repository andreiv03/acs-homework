#include "../include/graph.h"
#include "../include/priority-queue.h"
#include <stdio.h>

#define INFINITY 0x3f3f3f3f

int reconstructPath(FILE* outputFileStream, Graph* graph,
										int* previousVertices, int finishIndex) {
	int path[256];
	int pathLength = 0;
	int minimumDepth = INFINITY;

	int currentIndex = finishIndex;
	while (currentIndex != -1) {
		if (graph->adjacencyLists[currentIndex]->head->depth > 1)
			if (graph->adjacencyLists[currentIndex]->head->depth < minimumDepth)
				minimumDepth = graph->adjacencyLists[currentIndex]->head->depth;

		path[pathLength] = currentIndex;
		pathLength = pathLength + 1;
		currentIndex = previousVertices[currentIndex];
	}

	for (int index = pathLength - 1; index >= 0; --index) {
		Vertex* vertex = graph->adjacencyLists[path[index]]->head;
		fprintf(outputFileStream, "%s ", vertex->destination);
	}

	fprintf(outputFileStream, "\n");
	return minimumDepth;
}

void applyDijkstraAlgorithm(Graph* graph, int startIndex, int* distances,
														int* previousVertices) {
	int visited[graph->vertices];
	for (int index = 0; index < graph->vertices; ++index)
		visited[index] = 0;

	distances[startIndex] = 0;

	PriorityQueue* priorityQueue = createPriorityQueue(graph->vertices);
	pushToPriorityQueue(priorityQueue, startIndex, distances[startIndex]);

	while (isPriorityQueueEmpty(priorityQueue) == 0) {
		int currentIndex = popFromPriorityQueue(priorityQueue);
		Vertex* vertex = graph->adjacencyLists[currentIndex]->head;
		visited[currentIndex] = 1;

		vertex = vertex->next;
		while (vertex != NULL) {
			int index = getVertexIndex(graph, vertex->destination);

			if (distances[currentIndex] + vertex->cost < distances[index]) {
				distances[index] = distances[currentIndex] + vertex->cost;
				previousVertices[index] = currentIndex;

				if (visited[index] == 0) {
					pushToPriorityQueue(priorityQueue, index, distances[index]);
					visited[index] = 1;
				}
			}

			vertex = vertex->next;
		}
	}

	freePriorityQueue(priorityQueue);
}

void solveTaskTwo(FILE* outputFileStream, Graph* graph, int treasureWeight) {
	int distances[graph->vertices];
	for (int index = 0; index < graph->vertices; ++index)
		distances[index] = INFINITY;

	int previousVertices[graph->vertices];
	for (int index = 0; index < graph->vertices; ++index)
		previousVertices[index] = -1;

	int islandIndex = getVertexIndex(graph, "Insula");
	int shipIndex = getVertexIndex(graph, "Corabie");

	applyDijkstraAlgorithm(graph, islandIndex, distances, previousVertices);

	if (distances[shipIndex] == INFINITY) {
		fprintf(outputFileStream, "Echipajul nu poate ajunge la insula\n");
		return;
	}

	int minimumDepth = reconstructPath(outputFileStream, graph,
																		 previousVertices, shipIndex);

	fprintf(outputFileStream, "%d\n", distances[shipIndex]);
	fprintf(outputFileStream, "%d\n", minimumDepth);
	int totalRoutes = treasureWeight / minimumDepth;
	totalRoutes = totalRoutes + (treasureWeight % minimumDepth != 0);
	fprintf(outputFileStream, "%d\n", totalRoutes);
}
