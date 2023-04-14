#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>

typedef struct RGB {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} RGB;

RGB **readPixelsMatrix(FILE *inputFileStream, int pixelsSize);
void freePixelsMatrix(RGB **pixels, int pixelsSize);

unsigned long long calculateAverageColor(RGB **pixels, int pixelsSize, int x, int y, char *color);
double calculateSimilarityScore(RGB **pixels, int pixelsSize, int x, int y);

#endif
