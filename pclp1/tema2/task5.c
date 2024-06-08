#include "./utils.h"

struct Plane {
  int x, y;
  unsigned char direction, type;
  int speed;
};

void SolveTask5(void *info, int nr_avioane, int T, int nr_pct_coord, int *X, int *Y, int N) {
  void *copy_info = info;

  const short TYPE_ONE[4] = {1, 3, 1, 2};
  const short TYPE_TWO[5] = {1, 2, 4, 1, 3};

  for (short t = 0; t <= T; ++t) {
    int result = 0;

    for (int index = 0; index < nr_avioane; ++index) {
      struct Plane plane;

      plane.x = *(short *)info;
      info = info + sizeof(short);

      plane.y = *(short *)info;
      info = info + sizeof(short);

      plane.direction = *(unsigned char *)info;
      info = info + sizeof(unsigned char);

      plane.type = *(unsigned char *)info;
      info = info + sizeof(unsigned char);

      info = info + 3 * sizeof(unsigned char);

      plane.speed = *(int *)info;
      info = info + sizeof(int);

      const short TYPE_ARRAY_LENGTH = plane.type == '1' ? 4 : 5;
      const short *TYPE_ARRAY = plane.type == '1' ? TYPE_ONE : TYPE_TWO;

      switch (plane.direction) {
        case 'N': {
          plane.x = plane.x - t * plane.speed;
          short ok = 0;

          for (int i = 0; i < nr_pct_coord; ++i) {
            for (int j = 0; j < TYPE_ARRAY_LENGTH; ++j) {
              for (int k = 0; k < *(TYPE_ARRAY + j); ++k) {
                if (*(X + i) == plane.x + j && *(Y + i) == plane.y - k)
                  ok = 1;

                if (*(X + i) == plane.x + j && *(Y + i) == plane.y + k)
                  ok = 1;
              }
            }
          }

          if (ok)
            result = result + 1;

          break;
        }

        case 'E': {
          plane.y = plane.y + t * plane.speed;
          short ok = 0;

          for (int i = 0; i < nr_pct_coord; ++i) {
            for (int j = 0; j < TYPE_ARRAY_LENGTH; ++j) {
              for (int k = 0; k < *(TYPE_ARRAY + j); ++k) {
                if (*(X + i) == plane.x - k && *(Y + i) == plane.y - j)
                  ok = 1;

                if (*(X + i) == plane.x + k && *(Y + i) == plane.y - j)
                  ok = 1;
              }
            }
          }

          if (ok)
            result = result + 1;

          break;
        }

        case 'S': {
          plane.x = plane.x + t * plane.speed;
          short ok = 0;

          for (int i = 0; i < nr_pct_coord; ++i) {
            for (int j = 0; j < TYPE_ARRAY_LENGTH; ++j) {
              for (int k = 0; k < *(TYPE_ARRAY + j); ++k) {
                if (*(X + i) == plane.x - j && *(Y + i) == plane.y - k)
                  ok = 1;

                if (*(X + i) == plane.x - j && *(Y + i) == plane.y + k)
                  ok = 1;
              }
            }
          }

          if (ok)
            result = result + 1;

          break;
        }

        case 'W': {
          plane.y = plane.y - t * plane.speed;
          short ok = 0;

          for (int i = 0; i < nr_pct_coord; ++i) {
            for (int j = 0; j < TYPE_ARRAY_LENGTH; ++j) {
              for (int k = 0; k < *(TYPE_ARRAY + j); ++k) {
                if (*(X + i) == plane.x - k && *(Y + i) == plane.y + j)
                  ok = 1;

                if (*(X + i) == plane.x + k && *(Y + i) == plane.y + j)
                  ok = 1;
              }
            }
          }

          if (ok)
            result = result + 1;

          break;
        }

        default:
          break;
      }
    }

    printf("%d: %d\n", t, result);
    info = copy_info;
  }
}
