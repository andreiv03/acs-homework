#include "../include/graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Vertex* createVertex(char* destination, int cost) {
	Vertex* vertex = calloc(1, sizeof(Vertex));
	strcpy(vertex->destination, destination);
	vertex->cost = cost;
	return vertex;
}

int getVertexIndex(Graph* graph, char* destination) {
	for (int index = 0; index < graph->vertices; ++index) {
		Vertex* vertex = graph->adjacencyLists[index]->head;
		if (strcmp(vertex->destination, destination) == 0)
			return index;
	}

	return -1;
}

AdjacencyList* createAdjacencyList(char* source, Vertex* nextVertex) {
	AdjacencyList* adjacencyList = calloc(1, sizeof(AdjacencyList));
	Vertex* vertex = createVertex(source, 0);
	adjacencyList->head = vertex;
	adjacencyList->head->next = nextVertex;
	return adjacencyList;
}

void addVertexToAdjacencyList(AdjacencyList* adjacencyList, Vertex* vertex) {
	Vertex* currentVertex = adjacencyList->head;
	while (currentVertex->next != NULL)
		currentVertex = currentVertex->next;
	currentVertex->next = vertex;
}

Graph* createGraph(int vertices) {
	Graph* graph = calloc(1, sizeof(Graph));
	graph->vertices = vertices;
	graph->adjacencyLists = calloc(vertices, sizeof(AdjacencyList*));
	return graph;
}

void addEdgeToGraph(Graph* graph, char* source, char* destination, int cost) {
	Vertex* vertex = createVertex(destination, cost);

	for (int index = 0; index < graph->vertices; ++index) {
		if (graph->adjacencyLists[index] == NULL) {
			graph->adjacencyLists[index] = createAdjacencyList(source, vertex);
			break;
		}

		if (strcmp(graph->adjacencyLists[index]->head->destination, source) == 0) {
			addVertexToAdjacencyList(graph->adjacencyLists[index], vertex);
			break;
		}
	}
}

void freeGraph(Graph* graph) {
	for (int index = 0; index < graph->vertices; ++index) {
		Vertex* vertex = graph->adjacencyLists[index]->head;

		while (vertex != NULL) {
			Vertex* nextVertex = vertex->next;
			free(vertex);
			vertex = nextVertex;
		}

		free(graph->adjacencyLists[index]);
	}

	free(graph->adjacencyLists);
	free(graph);
}
