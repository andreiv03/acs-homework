#include "./include/quadtree.h"
#include "./include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const* argv[]) {
	FILE* inputFileStream;
	FILE* outputFileStream;

	inputFileStream = fopen(argv[argc - 2], "rb");
	if (inputFileStream == NULL)
		return 1;

	outputFileStream = fopen(argv[argc - 1], "wb");
	if (outputFileStream == NULL)
		return 1;

	if (strstr(argv[1], "-c")) {
		char garbage[255];
		uint pixelsSize = 0;
		fscanf(inputFileStream, "%s\n", garbage);
		fscanf(inputFileStream, "%d %d\n", &pixelsSize, &pixelsSize);
		fscanf(inputFileStream, "%s", garbage);
		fread(garbage, sizeof(char), 1, inputFileStream);

		RGB** pixels = readPixelsMatrix(inputFileStream, pixelsSize);
		QuadTree* tree = buildCompressedQuadTree(pixels, pixelsSize, 0, 0, atoi(argv[2]));

		if (strchr(argv[1], '1')) {
			int treeHeight = getTreeHeight(tree);
			fprintf(outputFileStream, "%d\n", treeHeight);

			int externalNodesNumber = getExternalNodesNumber(tree);
			fprintf(outputFileStream, "%d\n", externalNodesNumber);

			int lowestExternalNodeLevel = findLowestExternalNodeLevel(tree, 0);
			int biggestBlockSize = pixelsSize;
			for (int level = 0; level < lowestExternalNodeLevel; ++level)
				biggestBlockSize = biggestBlockSize / 2;
			fprintf(outputFileStream, "%d\n", biggestBlockSize);
		}

		if (strchr(argv[1], '2')) {
			fwrite(&pixelsSize, sizeof(int), 1, outputFileStream);
			levelOrderTraversal(tree, outputFileStream);
		}

		freePixelsMatrix(pixels, pixelsSize);
		freeQuadTree(tree);
	}

	if (strstr(argv[1], "-d")) {
		uint pixelsSize = 0;
		fread(&pixelsSize, sizeof(int), 1, inputFileStream);

		fprintf(outputFileStream, "P6\n");
		fprintf(outputFileStream, "%d %d\n", pixelsSize, pixelsSize);
		fprintf(outputFileStream, "255\n");

		QuadTree* tree = buildDecompressedQuadTree(inputFileStream);
		RGB** pixels = calloc(pixelsSize, sizeof(RGB*));
		for (uint row = 0; row < pixelsSize; ++row)
			pixels[row] = calloc(pixelsSize, sizeof(RGB));

		decompressQuadTree(tree, pixels, pixelsSize, 0, 0);

		for (uint row = 0; row < pixelsSize; ++row) {
			for (uint column = 0; column < pixelsSize; ++column) {
				fwrite(&pixels[row][column].red, sizeof(char), 1, outputFileStream);
				fwrite(&pixels[row][column].green, sizeof(char), 1, outputFileStream);
				fwrite(&pixels[row][column].blue, sizeof(char), 1, outputFileStream);
			}
		}

		freePixelsMatrix(pixels, pixelsSize);
		freeQuadTree(tree);
	}

	fclose(inputFileStream);
	fclose(outputFileStream);

	return 0;
}