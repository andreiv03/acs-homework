#include "../include/quadtree.h"
#include "../include/queue.h"
#include "../include/utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

QuadTree* buildCompressedQuadTree(RGB** pixels, uint pixelsSize, uint x, uint y, int factor) {
	QuadTree* tree = calloc(1, sizeof(QuadTree));
	RGB pixel;
	ull similarityScore = calculateSimilarityScore(pixels, pixelsSize, x, y, &pixel);

	if (similarityScore <= factor || pixelsSize == 1) {
		tree->type = 1;
		tree->rgb.red = pixel.red;
		tree->rgb.green = pixel.green;
		tree->rgb.blue = pixel.blue;
		return tree;
	}

	uint childSize = pixelsSize / 2;
	tree->type = 0;
	tree->topLeft = buildCompressedQuadTree(pixels, childSize, x, y, factor);
	tree->topRight = buildCompressedQuadTree(pixels, childSize, x, y + childSize, factor);
	tree->bottomRight = buildCompressedQuadTree(pixels, childSize, x + childSize, y + childSize, factor);
	tree->bottomLeft = buildCompressedQuadTree(pixels, childSize, x + childSize, y, factor);

	return tree;
}

QuadTree* buildDecompressedQuadTree(FILE* inputFileStream) {
	Queue* queue = calloc(1, sizeof(Queue));

	QuadTree* tree = calloc(1, sizeof(QuadTree));
	QuadTree* root = tree;
	fread(&tree->type, sizeof(char), 1, inputFileStream);

	if (tree->type == 1) {
		fread(&tree->rgb.red, sizeof(char), 1, inputFileStream);
		fread(&tree->rgb.green, sizeof(char), 1, inputFileStream);
		fread(&tree->rgb.blue, sizeof(char), 1, inputFileStream);
	}

	while (1) {
		QuadTree* child = calloc(1, sizeof(QuadTree));

		if (fread(&child->type, sizeof(char), 1, inputFileStream) == 0) {
			freeQuadTree(child);
			break;
		}

		if (child->type == 0)
			pushToQueue(queue, child);

		if (child->type == 1) {
			fread(&child->rgb.red, sizeof(char), 1, inputFileStream);
			fread(&child->rgb.green, sizeof(char), 1, inputFileStream);
			fread(&child->rgb.blue, sizeof(char), 1, inputFileStream);
		}

		if (isQuadTreeFull(tree)) {
			tree = queue->head->data;
			popFromQueue(queue);
		}

		if (tree->topLeft == NULL)
			tree->topLeft = child;
		else if (tree->topRight == NULL)
			tree->topRight = child;
		else if (tree->bottomRight == NULL)
			tree->bottomRight = child;
		else if (tree->bottomLeft == NULL)
			tree->bottomLeft = child;
	}

	free(queue);
	return root;
}

void freeQuadTree(QuadTree* tree) {
	if (tree == NULL)
		return;

	freeQuadTree(tree->topLeft);
	freeQuadTree(tree->topRight);
	freeQuadTree(tree->bottomRight);
	freeQuadTree(tree->bottomLeft);

	free(tree);
}

int getTreeHeight(QuadTree* tree) {
	if (tree == NULL)
		return 0;

	int topLeftTreeHeight = getTreeHeight(tree->topLeft);
	int topRightTreeHeight = getTreeHeight(tree->topRight);
	int bottomRightTreeHeight = getTreeHeight(tree->bottomRight);
	int bottomLeftTreeHeight = getTreeHeight(tree->bottomLeft);

	int maxHeight = topLeftTreeHeight;
	if (topRightTreeHeight > maxHeight)
		maxHeight = topRightTreeHeight;
	if (bottomRightTreeHeight > maxHeight)
		maxHeight = bottomRightTreeHeight;
	if (bottomLeftTreeHeight > maxHeight)
		maxHeight = bottomLeftTreeHeight;

	return maxHeight + 1;
}

int getExternalNodesNumber(QuadTree* tree) {
	if (tree == NULL)
		return 0;

	int number = 0;
	if (tree->type == 1)
		number = number + 1;

	number = number + getExternalNodesNumber(tree->topLeft);
	number = number + getExternalNodesNumber(tree->topRight);
	number = number + getExternalNodesNumber(tree->bottomRight);
	number = number + getExternalNodesNumber(tree->bottomLeft);

	return number;
}

int findLowestExternalNodeLevel(QuadTree* tree, int level) {
	if (tree == NULL)
		return 0;

	if (tree->type == 1)
		return level;

	int topLeftTreeLevel = findLowestExternalNodeLevel(tree->topLeft, level + 1);
	int topRightTreeLevel = findLowestExternalNodeLevel(tree->topRight, level + 1);
	int bottomRightTreeLevel = findLowestExternalNodeLevel(tree->bottomRight, level + 1);
	int bottomLeftTreeLevel = findLowestExternalNodeLevel(tree->bottomLeft, level + 1);

	int lowestLevel = topLeftTreeLevel;
	if (topRightTreeLevel < lowestLevel)
		lowestLevel = topRightTreeLevel;
	if (bottomRightTreeLevel < lowestLevel)
		lowestLevel = bottomRightTreeLevel;
	if (bottomLeftTreeLevel < lowestLevel)
		lowestLevel = bottomLeftTreeLevel;

	return lowestLevel;
}

void levelOrderTraversal(QuadTree* tree, FILE* outputFileStream) {
	Queue* queue = calloc(1, sizeof(Queue));
	pushToQueue(queue, tree);

	while (queue->head) {
		QuadTree* node = queue->head->data;
		popFromQueue(queue);

		if (node->type == 0) {
			fwrite(&node->type, sizeof(char), 1, outputFileStream);
			pushToQueue(queue, node->topLeft);
			pushToQueue(queue, node->topRight);
			pushToQueue(queue, node->bottomRight);
			pushToQueue(queue, node->bottomLeft);
		}

		if (node->type == 1) {
			fwrite(&node->type, sizeof(char), 1, outputFileStream);
			fwrite(&node->rgb.red, sizeof(char), 1, outputFileStream);
			fwrite(&node->rgb.green, sizeof(char), 1, outputFileStream);
			fwrite(&node->rgb.blue, sizeof(char), 1, outputFileStream);
		}
	}

	free(queue);
}

int isQuadTreeFull(QuadTree* tree) {
	if (tree->topLeft == NULL || tree->topRight == NULL)
		return 0;
	if (tree->bottomRight == NULL || tree->bottomLeft == NULL)
		return 0;
	return 1;
}

void decompressQuadTree(QuadTree* tree, RGB** pixels, uint pixelsSize, uint x, uint y) {
	if (tree->type == 0) {
		uint childSize = pixelsSize / 2;
		decompressQuadTree(tree->topLeft, pixels, childSize, x, y);
		decompressQuadTree(tree->topRight, pixels, childSize, x, y + childSize);
		decompressQuadTree(tree->bottomRight, pixels, childSize, x + childSize, y + childSize);
		decompressQuadTree(tree->bottomLeft, pixels, childSize, x + childSize, y);
	}

	if (tree->type == 1) {
		for (uint row = x; row < x + pixelsSize; ++row) {
			for (uint column = y; column < y + pixelsSize; ++column) {
				pixels[row][column].red = tree->rgb.red;
				pixels[row][column].green = tree->rgb.green;
				pixels[row][column].blue = tree->rgb.blue;
			}
		}
	}
}
