#ifndef QUEUE
#define QUEUE

#include "./list.h"

void pushToQueue(struct SinglyNode** head, struct SinglyNode** tail, char* data);
struct SinglyNode* popFromQueue(struct SinglyNode** head);

#endif