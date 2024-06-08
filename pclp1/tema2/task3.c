#include <stdlib.h>
#include <string.h>

#include "./utils.h"

struct Plane {
  unsigned char type, code[4];
  unsigned int speed;
};

int compare(const void *a, const void *b) {
  struct Plane planeA, planeB;

  a = a + 2 * sizeof(unsigned short) + sizeof(unsigned char);
  b = b + 2 * sizeof(unsigned short) + sizeof(unsigned char);

  planeA.type = *(unsigned char *)a;
  planeB.type = *(unsigned char *)b;
  a = a + sizeof(unsigned char);
  b = b + sizeof(unsigned char);

  memcpy(planeA.code, a, 3 * sizeof(unsigned char));
  memcpy(planeB.code, b, 3 * sizeof(unsigned char));
  planeA.code[3] = planeB.code[3] = '\0';
  a = a + 3 * sizeof(unsigned char);
  b = b + 3 * sizeof(unsigned char);

  planeA.speed = *(unsigned int *)a;
  planeB.speed = *(unsigned int *)b;

  if (planeA.type != planeB.type)
    return planeA.type - planeB.type;

  else if (strcmp(planeA.code, planeB.code))
    return strcmp(planeB.code, planeA.code);

  else
    return (int)(planeA.speed - planeB.speed);
}

void SolveTask3(void *info, int nr_avioane) {
  const short PLANE_SIZE = 13;
  qsort(info, nr_avioane, PLANE_SIZE, compare);
  SolveTask1(info, nr_avioane);
}
