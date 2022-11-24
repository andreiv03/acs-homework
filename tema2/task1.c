#include "./utils.h"

struct Plane {
  unsigned short x, y;
  unsigned char direction, type, code[4];
  unsigned int speed;
};

void SolveTask1(void *info, int nr_avioane) {
  for (int index = 0; index < nr_avioane; ++index) {
    struct Plane plane;

    plane.x = *(unsigned short *)info;
    info = info + sizeof(unsigned short);

    plane.y = *(unsigned short *)info;
    info = info + sizeof(unsigned short);

    plane.direction = *(unsigned char *)info;
    info = info + sizeof(unsigned char);

    plane.type = *(unsigned char *)info;
    info = info + sizeof(unsigned char);

    memcpy(plane.code, info, 3 * sizeof(unsigned char));
    plane.code[3] = '\0';
    info = info + 3 * sizeof(unsigned char);

    plane.speed = *(unsigned int *)info;
    info = info + sizeof(unsigned int);

    printf("(%hu, %hu)\n", plane.x, plane.y);
    printf("%c\n", plane.direction);
    printf("%c%s\n", plane.type, plane.code);
    printf("%d\n\n", plane.speed);
  }
}
