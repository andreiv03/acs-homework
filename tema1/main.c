#include <stdio.h>
#include <string.h>

#include "include/list.h"
#include "include/queue.h"
#include "include/stack.h"
#include "include/utils.h"

int main() {
	const char* inputFileName = "tema1.in";
	FILE* inputFile = fopen(inputFileName, "r");
	if (inputFile == NULL)
		return 1;

	const char* outputFileName = "tema1.out";
	FILE* outputFile = fopen(outputFileName, "w");
	if (outputFile == NULL)
		return 1;

	struct DoublyNode* listHead = createDoublyNode();
	listHead->data = calloc(1, sizeof(char));
	*(char*)listHead->data = '#';

	struct SinglyNode* queueHead = NULL;
	struct SinglyNode* queueTail = NULL;

	struct DoublyNode* undoStackTail = NULL;
	struct DoublyNode* redoStackTail = NULL;

	int pointer = 0;
	size_t length = 0;
	fscanf(inputFile, "%ld", &length);
	getc(inputFile);

	for (size_t index = 0; index < length; ++index) {
		char* buffer = NULL;
		size_t bufferSize = 0;
		if (getline(&buffer, &bufferSize, inputFile) == -1)
			return 1;

		buffer[strcspn(buffer, "\n")] = '\0';

		char operation[32], value;
		getOperation(buffer, operation, &value);

		switch (getOperationType(operation)) {
			case 1:
				pushToQueue(&queueHead, &queueTail, buffer);
				break;

			case 2:
				if (strcmp(operation, "SHOW") == 0)
					showDoublyList(outputFile, listHead, pointer);

				if (strcmp(operation, "SHOW_CURRENT") == 0)
					showDoublyListPointer(outputFile, listHead, pointer);

				break;

			case 3:
				if (strcmp(operation, "UNDO") == 0) {
					struct DoublyNode* node = popFromStack(&undoStackTail);
					pushToStack(&redoStackTail, &pointer);
					pointer = *(int*)node->data;
					freeDoublyNode(node);
				}

				if (strcmp(operation, "REDO") == 0) {
					struct DoublyNode* node = popFromStack(&redoStackTail);
					pushToStack(&undoStackTail, &pointer);
					pointer = *(int*)node->data;
					freeDoublyNode(node);
				}

				break;

			case 4:
				struct SinglyNode* node = popFromQueue(&queueHead);
				char operation[32], value;
				getOperation((char*)node->data, operation, &value);
				freeSinglyNode(node);

				if (strcmp(operation, "MOVE_LEFT") == 0 ||
						strcmp(operation, "MOVE_RIGHT") == 0 ||
						strcmp(operation, "MOVE_LEFT_CHAR") == 0 ||
						strcmp(operation, "MOVE_RIGHT_CHAR") == 0)
					pushToStack(&undoStackTail, &pointer);

				else
					clearStack(&undoStackTail), clearStack(&redoStackTail);

				applyUpdateOperation(outputFile, &listHead, &pointer, operation, &value);
				break;

			default:
				break;
		}
	}

	fclose(inputFile);
	fclose(outputFile);

	return 0;
}
