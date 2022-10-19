#include <stdio.h>

int main() {
  printf("Introduceti valoarea numarului natural N:\n");

  unsigned int N = 0;
  scanf("%d", &N);

  const unsigned int SEVEN = 7;
  const unsigned int ELEVEN = 11;

  for (unsigned int index = 1; index <= N; ++index) {
    if (index % SEVEN == 0 && index % ELEVEN == 0)
      printf("TrickOrTreat");

    else if (index % SEVEN == 0)
      printf("Trick");

    else if (index % ELEVEN == 0)
      printf("Treat");

    else
      printf("%d", index);

    if (index < N)
      printf(" ");
  }

  return 0;
}
