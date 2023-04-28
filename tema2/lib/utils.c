#include "../include/utils.h"
#include "../include/quadtree.h"
#include <math.h>
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

ull calculateAverageColor(RGB** pixels, uint pixelsSize, uint x, uint y, char* color) {
	ull averageColor = 0;

	for (uint row = x; row < x + pixelsSize; ++row) {
		for (uint column = y; column < y + pixelsSize; ++column) {
			if (strcmp(color, "red") == 0)
				averageColor = averageColor + pixels[row][column].red;
			if (strcmp(color, "green") == 0)
				averageColor = averageColor + pixels[row][column].green;
			if (strcmp(color, "blue") == 0)
				averageColor = averageColor + pixels[row][column].blue;
		}
	}

	averageColor = averageColor / (pixelsSize * pixelsSize);
	return averageColor;
}

ull calculateSimilarityScore(RGB** pixels, uint pixelsSize, uint x, uint y, RGB* pixel) {
	ull averageRed = calculateAverageColor(pixels, pixelsSize, x, y, "red");
	ull averageGreen = calculateAverageColor(pixels, pixelsSize, x, y, "green");
	ull averageBlue = calculateAverageColor(pixels, pixelsSize, x, y, "blue");
	ull score = 0;

	for (uint row = x; row < x + pixelsSize; ++row) {
		for (uint column = y; column < y + pixelsSize; ++column) {
			score = score + pow(averageRed - pixels[row][column].red, 2);
			score = score + pow(averageGreen - pixels[row][column].green, 2);
			score = score + pow(averageBlue - pixels[row][column].blue, 2);
		}
	}

	pixel->red = averageRed;
	pixel->green = averageGreen;
	pixel->blue = averageBlue;

	score = score / (3 * pixelsSize * pixelsSize);
	return score;
}
