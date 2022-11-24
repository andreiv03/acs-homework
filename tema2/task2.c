#include "./utils.h"

struct Plane {
  unsigned short x, y;
  unsigned char direction, type;
};

void SolveTask2(void *info, int nr_avioane, int N, char **mat) {
  const short TYPE_ONE[4] = {1, 3, 1, 2};
  const short TYPE_TWO[5] = {1, 2, 4, 1, 3};

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

    const short TYPE_ARRAY_LENGTH = plane.type == '1' ? 4 : 5;
    const short *TYPE_ARRAY = plane.type == '1' ? TYPE_ONE : TYPE_TWO;

    switch (plane.direction) {
      case 'N':
        for (int index = 0; index < TYPE_ARRAY_LENGTH; ++index)
          for (int length = 0; length < *(TYPE_ARRAY + index); ++length) {
            *(*(mat + plane.x + index) + plane.y + length) = '*';
            *(*(mat + plane.x + index) + plane.y - length) = '*';
          }

        break;

      case 'E':
        for (int index = 0; index < TYPE_ARRAY_LENGTH; ++index)
          for (int length = 0; length < *(TYPE_ARRAY + index); ++length) {
            *(*(mat + plane.x + length) + plane.y - index) = '*';
            *(*(mat + plane.x - length) + plane.y - index) = '*';
          }

        break;

      case 'S':
        for (int index = 0; index < TYPE_ARRAY_LENGTH; ++index)
          for (int length = 0; length < *(TYPE_ARRAY + index); ++length) {
            *(*(mat + plane.x - index) + plane.y + length) = '*';
            *(*(mat + plane.x - index) + plane.y - length) = '*';
          }

        break;

      case 'W':
        for (int index = 0; index < TYPE_ARRAY_LENGTH; ++index)
          for (int length = 0; length < *(TYPE_ARRAY + index); ++length) {
            *(*(mat + plane.x + length) + plane.y + index) = '*';
            *(*(mat + plane.x - length) + plane.y + index) = '*';
          }

        break;

      default:
        break;
    }

    info = info + 3 * sizeof(unsigned char) + sizeof(unsigned int);
  }

  for (int i = 0; i < N; ++i, printf("\n"))
    for (int j = 0; j < N; ++j)
      printf("%c", *(*(mat + i) + j));
}
