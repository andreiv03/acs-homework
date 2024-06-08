#ifndef _TASK_ONE_H_
#define _TASK_ONE_H_

#include "./graph.h"
#include <stdio.h>

void dfs(Graph* graph, int index, int* visited, Graph* connectedGraph);
int compareEdges(const void* a, const void* b);
int compareCosts(const void* a, const void* b);
int applyKruskalAlgorithm(Graph* graph);
void solveTaskOne(FILE* outputFileStream, Graph* graph);

#endif