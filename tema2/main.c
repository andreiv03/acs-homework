#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./include/quadtree.h"
#include "./include/utils.h"

int main(int argc, char const *argv[]) {
	FILE *inputFileStream;
	FILE *outputFileStream;

	inputFileStream = fopen(argv[argc - 2], "rb");
	if (inputFileStream == NULL)
		return 1;

	outputFileStream = fopen(argv[argc - 1], "wb");
	if (outputFileStream == NULL)
		return 1;

	if (strstr(argv[1], "-c")) {
		char magicIdentifier[3];
		fscanf(inputFileStream, "%s\n", magicIdentifier);

		int pixelsSize = 0;
		fscanf(inputFileStream, "%d %d\n", &pixelsSize, &pixelsSize);

		int colorMaxValue = 0;
		fscanf(inputFileStream, "%d\n", &colorMaxValue);

		RGB **pixels = readPixelsMatrix(inputFileStream, pixelsSize);
		QuadTree *tree = buildCompressedQuadTree(pixels, pixelsSize, 0, 0, atoi(argv[2]));

		if (strchr(argv[1], '1')) {
			int treeHeight = getTreeHeight(tree);
			fwrite(&treeHeight, sizeof(int), 1, outputFileStream);

			int externalNodesNumber = getExternalNodesNumber(tree);
			fwrite(&externalNodesNumber, sizeof(int), 1, outputFileStream);

			int lowestExternalNodeLevel = findLowestExternalNodeLevel(tree, 0);
			int biggestBlockSize = pixelsSize;
			for (int level = 0; level < lowestExternalNodeLevel; ++level)
				biggestBlockSize = biggestBlockSize / 2;
			fwrite(&biggestBlockSize, sizeof(int), 1, outputFileStream);
		}

		if (strchr(argv[1], '2')) {
			fwrite(&pixelsSize, sizeof(int), 1, outputFileStream);

			int treeHeight = getTreeHeight(tree);
			for (int level = 0; level < treeHeight; ++level)
				levelOrderTraversal(tree, level, outputFileStream);
		}

		freePixelsMatrix(pixels, pixelsSize);
		freeQuadTree(tree);
	}

	if (strstr(argv[1], "-d")) {
		int pixelsSize = 0;
		fread(&pixelsSize, sizeof(int), 1, inputFileStream);

		fprintf(outputFileStream, "P6\n");
		fprintf(outputFileStream, "%d %d\n", pixelsSize, pixelsSize);
		fprintf(outputFileStream, "255\n");

		QuadTree *tree = buildDecompressedQuadTree(inputFileStream);
		RGB **pixels = calloc(pixelsSize, sizeof(RGB *));
		for (int row = 0; row < pixelsSize; ++row)
			pixels[row] = calloc(pixelsSize, sizeof(RGB));

		decompressQuadTree(tree, pixels, pixelsSize, 0, 0);

		for (int row = 0; row < pixelsSize; ++row) {
			for (int column = 0; column < pixelsSize; ++column) {
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