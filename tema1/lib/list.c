#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/list.h"

struct SinglyNode *createSinglyNode() {
  struct SinglyNode *node = calloc(1, sizeof(struct SinglyNode));
  return node;
}

struct DoublyNode *createDoublyNode() {
  struct DoublyNode *node = calloc(1, sizeof(struct DoublyNode));
  return node;
}

void freeSinglyNode(struct SinglyNode *node) {
  free(node->data);
  free(node);
}

void freeDoublyNode(struct DoublyNode *node) {
  free(node->data);
  free(node);
}

void destroySinglyList(struct SinglyNode **head) {
  while (*head != NULL) {
    struct SinglyNode *node = *head;
    *head = (*head)->next;
    freeSinglyNode(node);
  }
}

void destroyDoublyList(struct DoublyNode **head) {
  while (*head != NULL) {
    struct DoublyNode *node = *head;
    *head = (*head)->next;
    freeDoublyNode(node);
  }
}

void pushSinglyNodeAtBeginning(struct SinglyNode **head, void *data, size_t dataSize) {
  struct SinglyNode *node = createSinglyNode();
  node->data = calloc(1, dataSize);
  memcpy(node->data, data, dataSize);

  if (*head == NULL) {
    *head = node;
    return;
  }

  node->next = *head;
  *head = node;
}

void pushDoublyNodeAtBeginning(struct DoublyNode **head, void *data, size_t dataSize) {
  struct DoublyNode *node = createDoublyNode();
  node->data = calloc(1, dataSize);
  memcpy(node->data, data, dataSize);

  if (*head == NULL) {
    *head = node;
    return;
  }

  node->next = *head;
  (*head)->prev = node;
  *head = node;
}

void pushSinglyNodeAtEnd(struct SinglyNode **head, void *data, size_t dataSize) {
  struct SinglyNode *node = createSinglyNode();
  node->data = calloc(1, dataSize);
  memcpy(node->data, data, dataSize);

  if (*head == NULL) {
    *head = node;
    return;
  }

  struct SinglyNode *temp = *head;
  while (temp->next != NULL)
    temp = temp->next;
  temp->next = node;
}

void pushDoublyNodeAtEnd(struct DoublyNode **head, void *data, size_t dataSize) {
  struct DoublyNode *node = createDoublyNode();
  node->data = calloc(1, dataSize);
  memcpy(node->data, data, dataSize);

  if (*head == NULL) {
    *head = node;
    return;
  }

  struct DoublyNode *temp = *head;
  while (temp->next != NULL)
    temp = temp->next;
  temp->next = node;
  node->prev = temp;
}

void printSinglyList(FILE *stream, struct SinglyNode *head,
                     struct SinglyNode *node,
                     void (*printFunction)(FILE *, void *, int)) {
  head = head->next;

  while (head != NULL) {
    if (head == node)
      (*printFunction)(stream, head->data, 1);
    else
      (*printFunction)(stream, head->data, 0);

    head = head->next;
  }

  fprintf(stream, "\n");
}

void printDoublyList(FILE *stream, struct DoublyNode *head,
                     struct DoublyNode *node,
                     void (*printFunction)(FILE *, void *, int)) {
  head = head->next;

  while (head != NULL) {
    if (head == node)
      (*printFunction)(stream, head->data, 1);
    else
      (*printFunction)(stream, head->data, 0);

    head = head->next;
  }

  fprintf(stream, "\n");
}
