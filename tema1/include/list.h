#ifndef LIST
#define LIST

#include <stdio.h>

struct SinglyNode {
  void *data;
  struct SinglyNode *next;
};

struct DoublyNode {
  void *data;
  struct DoublyNode *next;
  struct DoublyNode *prev;
};

struct SinglyNode *createSinglyNode();
struct DoublyNode *createDoublyNode();

void freeSinglyNode(struct SinglyNode *node);
void freeDoublyNode(struct DoublyNode *node);

void pushSinglyNodeAtEnd(struct SinglyNode **head, void *data, size_t dataSize);
void pushDoublyNodeAtEnd(struct DoublyNode **head, void *data, size_t dataSize);

void printSinglyList(FILE *stream, struct SinglyNode *head,
                     struct SinglyNode *node,
                     void (*printFunction)(FILE *, void *, int));
void printDoublyList(FILE *stream, struct DoublyNode *head,
                     struct DoublyNode *node,
                     void (*printFunction)(FILE *, void *, int));

#endif
