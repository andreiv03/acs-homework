#ifndef _TASK_TWO_H_
#define _TASK_TWO_H_

#include "./graph.h"
#include <stdio.h>

int reconstructPath(FILE* outputFileStream, Graph* graph,
										int* previousVertices, int finishIndex);
void applyDijkstraAlgorithm(Graph* graph, int startIndex, int* distances,
														int* previousVertices);
void solveTaskTwo(FILE* outputFileStream, Graph* graph, int treasureWeight);

#endif
