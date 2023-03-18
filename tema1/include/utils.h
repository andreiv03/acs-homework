#ifndef UTILS
#define UTILS

#include <stdio.h>

#include "./list.h"

void getOperation(char* string, char* operation, char* value);
int getOperationType(char* operation);
void applyUpdateOperation(FILE* file, struct DoublyNode** head, int* pointer, char* operation, char* value);

#endif
