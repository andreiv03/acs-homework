#include "../include/graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dfs(Graph* graph, int index, int* visited, Graph* connectedGraph) {
	visited[index] = 1;

	AdjacencyList* adjacencyList = graph->adjacencyLists[index];
	Vertex* currentVertex = adjacencyList->head->next;

	connectedGraph->vertices = connectedGraph->vertices + 1;
	int connectedGraphSize = connectedGraph->vertices * sizeof(AdjacencyList*);
	connectedGraph->adjacencyLists = realloc(connectedGraph->adjacencyLists,
																					 connectedGraphSize);
	connectedGraph->adjacencyLists[connectedGraph->vertices - 1] = adjacencyList;

	while (currentVertex != NULL) {
		int currentVertexIndex = getVertexIndex(graph, currentVertex->destination);

		if (visited[currentVertexIndex] == 0)
			dfs(graph, currentVertexIndex, visited, connectedGraph);

		currentVertex = currentVertex->next;
	}
}

int compareEdges(const void* a, const void* b) {
	Edge* firstEdge = (Edge*)a;
	Edge* secondEdge = (Edge*)b;
	return firstEdge->cost - secondEdge->cost;
}

int compareCosts(const void* a, const void* b) {
	int firstCost = *(int*)a;
	int secondCost = *(int*)b;
	return firstCost - secondCost;
}

int applyKruskalAlgorithm(Graph* graph) {
	int edgesLength = 0;
	Edge* edges = calloc(edgesLength, sizeof(Edge));

	for (int index = 0; index < graph->vertices; ++index) {
		AdjacencyList* adjacencyList = graph->adjacencyLists[index];
		Vertex* currentVertex = adjacencyList->head->next;

		while (currentVertex != NULL) {
			edges = realloc(edges, (edgesLength + 1) * sizeof(Edge));
			strcpy(edges[edgesLength].source, adjacencyList->head->destination);
			strcpy(edges[edgesLength].destination, currentVertex->destination);
			edges[edgesLength].cost = currentVertex->cost;
			edgesLength = edgesLength + 1;
			currentVertex = currentVertex->next;
		}
	}

	qsort(edges, edgesLength, sizeof(Edge), compareEdges);

	int parent[graph->vertices];
	for (int index = 0; index < graph->vertices; ++index)
		parent[index] = index;

	Edge* minimumSpanningTree = calloc(graph->vertices - 1, sizeof(Edge));
	int minimumSpanningTreeLength = 0;

	for (int index = 0; index < edgesLength; ++index) {
		if (minimumSpanningTreeLength == graph->vertices - 1)
			break;

		int sourceIndex = getVertexIndex(graph, edges[index].source);
		int destinationIndex = getVertexIndex(graph, edges[index].destination);

		if (parent[sourceIndex] != parent[destinationIndex]) {
			minimumSpanningTree[minimumSpanningTreeLength] = edges[index];
			minimumSpanningTreeLength = minimumSpanningTreeLength + 1;

			for (int index = 0; index < graph->vertices; ++index)
				if (parent[index] == parent[destinationIndex])
					parent[index] = parent[sourceIndex];
		}
	}

	int cost = 0;
	for (int index = 0; index < minimumSpanningTreeLength; ++index)
		cost = cost + minimumSpanningTree[index].cost;

	free(edges);
	free(minimumSpanningTree);

	return cost;
}

void solveTaskOne(FILE* outputFileStream, Graph* graph) {
	int connectedGraphs = 0;
	int* costs = calloc(connectedGraphs, sizeof(int));

	int visited[graph->vertices];
	for (int index = 0; index < graph->vertices; ++index)
		visited[index] = 0;

	for (int index = 0; index < graph->vertices; ++index) {
		if (visited[index] == 0) {
			Graph* connectedGraph = createGraph(0);
			dfs(graph, index, visited, connectedGraph);

			connectedGraphs = connectedGraphs + 1;
			costs = realloc(costs, connectedGraphs * sizeof(int));
			costs[connectedGraphs - 1] = applyKruskalAlgorithm(connectedGraph);

			free(connectedGraph->adjacencyLists);
			free(connectedGraph);
		}
	}

	qsort(costs, connectedGraphs, sizeof(int), compareCosts);

	fprintf(outputFileStream, "%d\n", connectedGraphs);
	for (int index = 0; index < connectedGraphs; ++index)
		fprintf(outputFileStream, "%d\n", costs[index]);

	free(costs);
}
