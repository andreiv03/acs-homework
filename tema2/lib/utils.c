#include "../include/utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/quadtree.h"

RGB **readPixelsMatrix(FILE *inputFileStream, int pixelsSize) {
	RGB **pixels = calloc(pixelsSize, sizeof(RGB *));

	for (int row = 0; row < pixelsSize; ++row) {
		pixels[row] = calloc(pixelsSize, sizeof(RGB));

		for (int column = 0; column < pixelsSize; ++column) {
			fread(&pixels[row][column].red, sizeof(char), 1, inputFileStream);
			fread(&pixels[row][column].green, sizeof(char), 1, inputFileStream);
			fread(&pixels[row][column].blue, sizeof(char), 1, inputFileStream);
		}
	}

	return pixels;
}

void freePixelsMatrix(RGB **pixels, int pixelsSize) {
	for (int row = 0; row < pixelsSize; ++row)
		free(pixels[row]);
	free(pixels);
}

double calculateAverageColor(RGB **pixels, int pixelsSize, int x, int y, char *color) {
	double averageColor = 0;

	for (int row = x; row < x + pixelsSize; ++row) {
		for (int column = y; column < y + pixelsSize; ++column) {
			if (strstr(color, "red"))
				averageColor = averageColor + pixels[row][column].red;
			if (strstr(color, "green"))
				averageColor = averageColor + pixels[row][column].green;
			if (strstr(color, "blue"))
				averageColor = averageColor + pixels[row][column].blue;
		}
	}

	averageColor = averageColor / (pixelsSize * pixelsSize);
	return averageColor;
}

double calculateSimilarityScore(RGB **pixels, int pixelsSize, int x, int y) {
	double averageRed = calculateAverageColor(pixels, pixelsSize, x, y, "red");
	double averageGreen = calculateAverageColor(pixels, pixelsSize, x, y, "green");
	double averageBlue = calculateAverageColor(pixels, pixelsSize, x, y, "blue");
	double score = 0;

	for (int row = x; row < x + pixelsSize; ++row) {
		for (int column = y; column < y + pixelsSize; ++column) {
			score = score + pow(averageRed - pixels[row][column].red, 2);
			score = score + pow(averageGreen - pixels[row][column].green, 2);
			score = score + pow(averageBlue - pixels[row][column].blue, 2);
		}
	}

	score = score / (3 * pixelsSize * pixelsSize);
	return score;
}
