#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/list.h"
#include "../include/utils.h"

void printChar(FILE *stream, void *data, int isHighlighted) {
  if (isHighlighted)
    fprintf(stream, "|%c|", *(char *)data);
  else
    fprintf(stream, "%c", *(char *)data);
}

int getOperationType(char *operation) {
  if (strcmp(operation, "MOVE_LEFT") == 0 ||
      strcmp(operation, "MOVE_RIGHT") == 0 ||
      strcmp(operation, "MOVE_LEFT_CHAR") == 0 ||
      strcmp(operation, "MOVE_RIGHT_CHAR") == 0 ||
      strcmp(operation, "WRITE") == 0 ||
      strcmp(operation, "INSERT_LEFT") == 0 ||
      strcmp(operation, "INSERT_RIGHT") == 0)
    return 1;

  if (strcmp(operation, "SHOW") == 0 || strcmp(operation, "SHOW_CURRENT") == 0)
    return 2;

  if (strcmp(operation, "UNDO") == 0 || strcmp(operation, "REDO") == 0)
    return 3;

  if (strcmp(operation, "EXECUTE") == 0)
    return 4;

  return 0;
}

int applyUpdateOperation(FILE *stream, struct Band **band, char *operation,
                         char *value) {
  if (strcmp(operation, "MOVE_LEFT") == 0) {
    if ((*band)->finger != (*band)->list->next)
      (*band)->finger = (*band)->finger->prev;
    else
      return 1;
  }

  if (strcmp(operation, "MOVE_RIGHT") == 0) {
    if ((*band)->finger->next == NULL) {
      void *data = calloc(1, sizeof(char));
      *(char *)data = '#';
      pushDoublyNodeAtEnd(&(*band)->list, data, sizeof(char));
      free(data);
    }

    (*band)->finger = (*band)->finger->next;
  }

  if (strcmp(operation, "MOVE_LEFT_CHAR") == 0) {
    struct DoublyNode *temp = (*band)->finger;

    while ((*band)->finger->data && *(char *)(*band)->finger->data != *value)
      (*band)->finger = (*band)->finger->prev;

    if ((*band)->finger->data == NULL) {
      (*band)->finger = temp;
      fprintf(stream, "%s\n", "ERROR");
    }
  }

  if (strcmp(operation, "MOVE_RIGHT_CHAR") == 0) {
    while ((*band)->finger->next && *(char *)(*band)->finger->data != *value)
      (*band)->finger = (*band)->finger->next;

    if (*(char *)(*band)->finger->data != *value) {
      void *data = calloc(1, sizeof(char));
      *(char *)data = '#';
      pushDoublyNodeAtEnd(&(*band)->list, data, sizeof(char));
      free(data);
      (*band)->finger = (*band)->finger->next;
    }
  }

  if (strcmp(operation, "WRITE") == 0)
    *(char *)(*band)->finger->data = *value;

  if (strcmp(operation, "INSERT_LEFT") == 0) {
    if ((*band)->finger->prev->data == NULL)
      fprintf(stream, "%s\n", "ERROR");
    else {
      struct DoublyNode *node = createDoublyNode();
      node->data = calloc(1, sizeof(char));
      *(char *)node->data = *value;
      node->next = (*band)->finger;
      node->prev = (*band)->finger->prev;
      (*band)->finger->prev->next = node;
      (*band)->finger->prev = node;
      (*band)->finger = (*band)->finger->prev;
    }
  }

  if (strcmp(operation, "INSERT_RIGHT") == 0) {
    struct DoublyNode *node = createDoublyNode();
    node->data = calloc(1, sizeof(char));
    *(char *)node->data = *value;
    node->prev = (*band)->finger;

    if ((*band)->finger->next) {
      node->next = (*band)->finger->next;
      (*band)->finger->next->prev = node;
    }

    (*band)->finger->next = node;
    (*band)->finger = (*band)->finger->next;
  }

  return 0;
}
