#ifndef _GRAPH_H_
#define _GRAPH_H_

typedef struct Vertex {
	char destination[256];
	int cost;
	int depth;
	struct Vertex* next;
} Vertex;

typedef struct Edge {
	char source[256];
	char destination[256];
	int cost;
} Edge;

typedef struct AdjacencyList {
	Vertex* head;
} AdjacencyList;

typedef struct Graph {
	int vertices;
	AdjacencyList** adjacencyLists;
} Graph;

Vertex* createVertex(char* destination, int cost);
int getVertexIndex(Graph* graph, char* destination);
AdjacencyList* createAdjacencyList(char* source, Vertex* nextVertex);
void addVertexToAdjacencyList(AdjacencyList* adjacencyList, Vertex* vertex);
Graph* createGraph(int vertices);
void addEdgeToGraph(Graph* graph, char* source, char* destination, int cost);
void freeGraph(Graph* graph);

#endif
