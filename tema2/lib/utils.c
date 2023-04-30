#include "../include/utils.h"
#include "../include/quadtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

RGB** readPixelsMatrix(FILE* inputFileStream, uint pixelsSize) {
	RGB** pixels = calloc(pixelsSize, sizeof(RGB*));

	for (uint row = 0; row < pixelsSize; ++row) {
		pixels[row] = calloc(pixelsSize, sizeof(RGB));

		for (uint column = 0; column < pixelsSize; ++column) {
			fread(&pixels[row][column].red, sizeof(char), 1, inputFileStream);
			fread(&pixels[row][column].green, sizeof(char), 1, inputFileStream);
			fread(&pixels[row][column].blue, sizeof(char), 1, inputFileStream);
		}
	}

	return pixels;
}

void freePixelsMatrix(RGB** pixels, uint pixelsSize) {
	for (uint row = 0; row < pixelsSize; ++row)
		free(pixels[row]);
	free(pixels);
}

ull calculateSimilarityScore(RGB** pixels, uint pixelsSize, uint x, uint y, RGB* pixel) {
	ull averageRed = 0;
	ull averageGreen = 0;
	ull averageBlue = 0;

	for (uint row = x; row < x + pixelsSize; ++row) {
		for (uint column = y; column < y + pixelsSize; ++column) {
			averageRed = averageRed + pixels[row][column].red;
			averageGreen = averageGreen + pixels[row][column].green;
			averageBlue = averageBlue + pixels[row][column].blue;
		}
	}

	averageRed = averageRed / (pixelsSize * pixelsSize);
	averageGreen = averageGreen / (pixelsSize * pixelsSize);
	averageBlue = averageBlue / (pixelsSize * pixelsSize);

	ull score = 0;

	for (uint row = x; row < x + pixelsSize; ++row) {
		for (uint column = y; column < y + pixelsSize; ++column) {
			score = score + (averageRed - pixels[row][column].red) *
													(averageRed - pixels[row][column].red);
			score = score + (averageGreen - pixels[row][column].green) *
													(averageGreen - pixels[row][column].green);
			score = score + (averageBlue - pixels[row][column].blue) *
													(averageBlue - pixels[row][column].blue);
		}
	}

	pixel->red = averageRed;
	pixel->green = averageGreen;
	pixel->blue = averageBlue;

	score = score / (3 * pixelsSize * pixelsSize);
	return score;
}
