#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "./utils.h"
#include <stdio.h>

typedef struct QuadTree {
	uchar type;
	RGB rgb;

	struct QuadTree* topLeft;
	struct QuadTree* topRight;
	struct QuadTree* bottomRight;
	struct QuadTree* bottomLeft;
} QuadTree;

QuadTree* buildCompressedQuadTree(RGB** pixels, uint pixelsSize, uint x, uint y, int factor);
QuadTree* buildDecompressedQuadTree(FILE* inputFileStream);
void freeQuadTree(QuadTree* tree);

int getTreeHeight(QuadTree* tree);
int getExternalNodesNumber(QuadTree* tree);
int findLowestExternalNodeLevel(QuadTree* tree, int level);
void levelOrderTraversal(QuadTree* tree, FILE* outputFileStream);
int isQuadTreeFull(QuadTree* tree);
void decompressQuadTree(QuadTree* tree, RGB** pixels, uint pixelsSize, uint x, uint y);

#endif
