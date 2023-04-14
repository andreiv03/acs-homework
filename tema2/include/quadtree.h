#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include <stdio.h>

#include "./utils.h"

typedef struct QuadTree {
	unsigned char type;
	RGB rgb;

	struct QuadTree *topLeft;
	struct QuadTree *topRight;
	struct QuadTree *bottomRight;
	struct QuadTree *bottomLeft;
} QuadTree;

QuadTree *buildCompressedQuadTree(RGB **pixels, int pixelsSize, int x, int y, int factor);
QuadTree *buildDecompressedQuadTree(FILE *inputFileStream);
void freeQuadTree(QuadTree *tree);

int getTreeHeight(QuadTree *tree);
int getExternalNodesNumber(QuadTree *tree);
int findLowestExternalNodeLevel(QuadTree *tree, int level);
void levelOrderTraversal(QuadTree *tree, int level, FILE *outputFileStream);
int isQuadTreeFull(QuadTree *tree);
void decompressQuadTree(QuadTree *tree, RGB **pixels, int pixelsSize, int x, int y);

#endif
