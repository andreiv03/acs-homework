#include "../include/graph.h"
#include "../include/priority-queue.h"
#include <stdio.h>

#define INFINITY 0x3f3f3f3f

int reconstructPath(FILE* outputFileStream, Graph* graph, int* parents, int finishIndex) {
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
		currentIndex = parents[currentIndex];
	}

	for (int index = pathLength - 1; index >= 0; --index) {
		Vertex* vertex = graph->adjacencyLists[path[index]]->head;
		fprintf(outputFileStream, "%s ", vertex->destination);
	}

	fprintf(outputFileStream, "\n");
	return minimumDepth;
}

void applyDijkstraAlgorithm(Graph* graph, int startIndex, int* distances, int* parents) {
	int visited[graph->vertices];
	float scores[graph->vertices];

	for (int index = 0; index < graph->vertices; ++index) {
		visited[index] = 0;
		scores[index] = INFINITY;
	}

	distances[startIndex] = 0;
	scores[startIndex] = 0.0;

	PriorityQueue* priorityQueue = createPriorityQueue(graph->vertices);
	pushToPriorityQueue(priorityQueue, startIndex, distances[startIndex]);

	while (isPriorityQueueEmpty(priorityQueue) == 0) {
		int currentIndex = popFromPriorityQueue(priorityQueue);
		Vertex* vertex = graph->adjacencyLists[currentIndex]->head;
		visited[currentIndex] = 1;

		while (vertex != NULL) {
			int index = getVertexIndex(graph, vertex->destination);
			float score = 1.0 * vertex->cost / graph->adjacencyLists[index]->head->depth;

			if (scores[currentIndex] + score < scores[index]) {
				scores[index] = scores[currentIndex] + score;
				distances[index] = distances[currentIndex] + vertex->cost;
				parents[index] = currentIndex;

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
	int distancesFromIsland[graph->vertices];
	int distancesFromShip[graph->vertices];
	int parentsFromIsland[graph->vertices];
	int parentsFromShip[graph->vertices];

	for (int index = 0; index < graph->vertices; ++index) {
		distancesFromIsland[index] = INFINITY;
		distancesFromShip[index] = INFINITY;
		parentsFromIsland[index] = -1;
		parentsFromShip[index] = -1;
	}

	int islandIndex = getVertexIndex(graph, "Insula");
	int shipIndex = getVertexIndex(graph, "Corabie");

	applyDijkstraAlgorithm(graph, islandIndex, distancesFromIsland, parentsFromIsland);
	if (distancesFromIsland[shipIndex] == INFINITY) {
		fprintf(outputFileStream, "Echipajul nu poate transporta comoara inapoi la corabie\n");
		return;
	}

	applyDijkstraAlgorithm(graph, shipIndex, distancesFromShip, parentsFromShip);
	if (distancesFromShip[islandIndex] == INFINITY) {
		fprintf(outputFileStream, "Echipajul nu poate ajunge la insula\n");
		return;
	}

	int minimumDepth = reconstructPath(outputFileStream, graph, parentsFromIsland, shipIndex);
	fprintf(outputFileStream, "%d\n", distancesFromIsland[shipIndex]);
	fprintf(outputFileStream, "%d\n", minimumDepth);
	fprintf(outputFileStream, "%d\n", treasureWeight / minimumDepth);
}
