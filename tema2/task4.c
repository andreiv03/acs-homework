#include "./utils.h"

struct Plane {
  unsigned short x, y;
  unsigned char direction, type;
};

void SolveTask4(void *info, int nr_avioane, int nr_obstacole, int *x, int *y, int N) {
  int result = 0;

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

    const short OFFSET = plane.type == '1' ? 0 : 1;
    const short DISTANCE = plane.type == '1' ? 2 : 3;

    switch (plane.direction) {
      case 'N': {
        short ok = 1;

        for (int index = 0; index < nr_obstacole; ++index) {
          if (*(x + index) <= plane.x + OFFSET + 1) {
            if (*(y + index) >= plane.y - DISTANCE && *(y + index) <= plane.y + DISTANCE)
              ok = 0;
          } else if (*(x + index) == plane.x + OFFSET + 2) {
            if (*(y + index) >= plane.y - DISTANCE + 1 && *(y + index) <= plane.y + DISTANCE - 1)
              ok = 0;
          }
        }

        if (ok)
          result = result + 1;

        break;
      }

      case 'E': {
        short ok = 1;

        for (int index = 0; index < nr_obstacole; ++index) {
          if (*(y + index) >= plane.y - OFFSET - 1) {
            if (*(x + index) >= plane.x - DISTANCE && *(x + index) <= plane.x + DISTANCE)
              ok = 0;
          } else if (*(y + index) == plane.y - OFFSET - 2) {
            if (*(x + index) >= plane.x - DISTANCE + 1 && *(x + index) <= plane.x + DISTANCE - 1)
              ok = 0;
          }
        }

        if (ok)
          result = result + 1;

        break;
      }

      case 'S': {
        short ok = 1;

        for (int index = 0; index < nr_obstacole; ++index) {
          if (*(x + index) >= plane.x - OFFSET - 1) {
            if (*(y + index) >= plane.y - DISTANCE && *(y + index) <= plane.y + DISTANCE)
              ok = 0;
          } else if (*(x + index) == plane.x - OFFSET - 2) {
            if (*(y + index) >= plane.y - DISTANCE + 1 && *(y + index) <= plane.y + DISTANCE - 1)
              ok = 0;
          }
        }

        if (ok)
          result = result + 1;

        break;
      }

      case 'W': {
        short ok = 1;

        for (int index = 0; index < nr_obstacole; ++index) {
          if (*(y + index) <= plane.y + OFFSET + 1) {
            if (*(x + index) >= plane.x - DISTANCE && *(x + index) <= plane.x + DISTANCE)
              ok = 0;
          } else if (*(y + index) == plane.y + 2 + OFFSET) {
            if (*(x + index) >= plane.x - DISTANCE + 1 && *(x + index) <= plane.x + DISTANCE - 1)
              ok = 0;
          }
        }

        if (ok)
          result = result + 1;

        break;
      }

      default:
        break;
    }

    info = info + 3 * sizeof(unsigned char) + sizeof(unsigned int);
  }

  printf("%d\n", result);
}
