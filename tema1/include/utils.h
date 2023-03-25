#ifndef UTILS
#define UTILS

struct Band {
  struct DoublyNode *finger;
  struct DoublyNode *list;
};

void printChar(FILE *stream, void *data, int isHighlighted);

int getOperationType(char *operation);
void applyUpdateOperation(FILE *stream, struct Band **band, char *operation,
                          char *value);

#endif
