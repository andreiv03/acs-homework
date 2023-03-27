#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/list.h"
#include "include/queue.h"
#include "include/stack.h"
#include "include/utils.h"

int main() {
  const char *inputFileName = "tema1.in";
  FILE *inputFileStream = fopen(inputFileName, "r");
  if (inputFileStream == NULL)
    return 1;

  const char *outputFileName = "tema1.out";
  FILE *outputFileStream = fopen(outputFileName, "w");
  if (outputFileStream == NULL)
    return 1;

  struct Band *band = calloc(1, sizeof(struct Band));
  band->list = createDoublyNode();

  void *data = calloc(1, sizeof(char));
  *(char *)data = '#';
  pushDoublyNodeAtEnd(&band->list, data, sizeof(char));
  free(data);

  band->finger = band->list->next;

  struct Queue *operationsQueue = calloc(1, sizeof(struct Queue));
  struct Queue *valuesQueue = calloc(1, sizeof(struct Queue));

  struct Stack *undoStack = calloc(1, sizeof(struct Stack));
  struct Stack *redoStack = calloc(1, sizeof(struct Stack));

  char line[100];
  fgets(line, sizeof(line), inputFileStream);
  size_t length = atoi(line);

  for (size_t index = 0; index < length; ++index) {
    char operation[20], value = 0;
    fgets(line, sizeof(line), inputFileStream);
    sscanf(line, "%s %c", operation, &value);

    switch (getOperationType(operation)) {
      case 1:
        pushToQueue(&operationsQueue, operation,
                    (strlen(operation) + 1) * sizeof(char));

        if (value) {
          pushToQueue(&valuesQueue, &value, sizeof(char));
          value = 0;
        }

        break;

      case 2:
        if (strcmp(operation, "SHOW") == 0)
          printDoublyList(outputFileStream, band->list, band->finger, printChar);

        if (strcmp(operation, "SHOW_CURRENT") == 0)
          fprintf(outputFileStream, "%c\n", *(char *)band->finger->data);

        break;

      case 3:
        if (strcmp(operation, "UNDO") == 0) {
          struct DoublyNode **node = popFromStack(&undoStack);
          pushToStack(&redoStack, &band->finger, sizeof(struct DoublyNode *));
          (*band).finger = *node;
          free(node);
        }

        if (strcmp(operation, "REDO") == 0) {
          struct DoublyNode **node = popFromStack(&redoStack);
          pushToStack(&undoStack, &band->finger, sizeof(struct DoublyNode *));
          (*band).finger = *node;
          free(node);
        }

        break;

      case 4:
        void *data = popFromQueue(&operationsQueue);
        void *value = NULL;
        if (strcmp((char *)data, "MOVE_LEFT") != 0 &&
            strcmp((char *)data, "MOVE_RIGHT") != 0)
          value = popFromQueue(&valuesQueue);

        if (strcmp((char *)data, "MOVE_LEFT") == 0 ||
            strcmp((char *)data, "MOVE_RIGHT") == 0)
          pushToStack(&undoStack, &band->finger, sizeof(struct DoublyNode *));
        else
          clearStack(&undoStack), clearStack(&redoStack);

        applyUpdateOperation(outputFileStream, &band, data, value);
        break;

      default:
        break;
    }
  }

  free(operationsQueue);
  free(valuesQueue);

  clearStack(&undoStack);
  clearStack(&redoStack);
  free(undoStack);
  free(redoStack);

  destroyDoublyList(&band->list);
  free(band);

  fclose(inputFileStream);
  fclose(outputFileStream);

  return 0;
}
