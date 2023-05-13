#include "./include/graph.h"
#include "./include/task-one.h"
#include "./include/task-two.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const* argv[]) {
	FILE* inputFileStream = fopen("tema3.in", "r");
	if (inputFileStream == NULL)
		return 1;

	FILE* outputFileStream = fopen("tema3.out", "w");
	if (outputFileStream == NULL)
		return 1;

	int task = atoi(argv[1]);
	if (task != 1 && task != 2)
		return 1;

	if (task == 1) {
		int vertices = 0;
		fscanf(inputFileStream, "%d", &vertices);
		int edges = 0;
		fscanf(inputFileStream, "%d", &edges);

		Graph* graph = createGraph(vertices);

		for (int index = 0; index < edges; ++index) {
			char source[256];
			fscanf(inputFileStream, "%s", source);
			char destination[256];
			fscanf(inputFileStream, "%s", destination);
			int cost = 0;
			fscanf(inputFileStream, "%d", &cost);

			addEdgeToGraph(graph, source, destination, cost);
			addEdgeToGraph(graph, destination, source, cost);
		}

		solveTaskOne(outputFileStream, graph);
		freeGraph(graph);
	}

	if (task == 2) {
		int vertices = 0;
		fscanf(inputFileStream, "%d", &vertices);
		int edges = 0;
		fscanf(inputFileStream, "%d", &edges);

		Graph* graph = createGraph(vertices);

		for (int index = 0; index < edges; ++index) {
			char source[256];
			fscanf(inputFileStream, "%s", source);
			char destination[256];
			fscanf(inputFileStream, "%s", destination);
			int cost = 0;
			fscanf(inputFileStream, "%d", &cost);

			addEdgeToGraph(graph, source, destination, cost);
		}

		for (int index = 0; index < vertices; ++index) {
			char source[256];
			fscanf(inputFileStream, "%s", source);

			int depth = 0;
			fscanf(inputFileStream, "%d", &depth);

			for (int index = 0; index < graph->vertices; ++index) {
				if (graph->adjacencyLists[index] == NULL) {
					graph->adjacencyLists[index] = createAdjacencyList(source, NULL);
					graph->adjacencyLists[index]->head->depth = depth;
					break;
				}

				Vertex* vertex = graph->adjacencyLists[index]->head;
				if (strcmp(vertex->destination, source) == 0) {
					graph->adjacencyLists[index]->head->depth = depth;
					break;
				}
			}
		}

		int treasureWeight = 0;
		fscanf(inputFileStream, "%d", &treasureWeight);

		solveTaskTwo(outputFileStream, graph, treasureWeight);
		freeGraph(graph);
	}

	fclose(inputFileStream);
	fclose(outputFileStream);

	return 0;
}
