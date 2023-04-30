#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long long ull;

typedef struct RGB {
	uchar red;
	uchar green;
	uchar blue;
} RGB;

RGB** readPixelsMatrix(FILE* inputFileStream, uint pixelsSize);
void freePixelsMatrix(RGB** pixels, uint pixelsSize);

ull calculateSimilarityScore(RGB** pixels, uint pixelsSize, uint x, uint y, RGB* pixel);

#endif
