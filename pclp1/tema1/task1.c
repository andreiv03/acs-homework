#include <stdio.h>

#include "utils.h"

int main() {
  int N = 0;
  scanf("%d", &N);

  float operands[N];
  for (int index = 0; index < N; ++index)
    scanf("%f", &operands[index]);

  char operations[N];
  scanf("%s", operations);

  float result = operands[0];
  for (int index = 0; index < N - 1; ++index)
    result = applyOperation(operations[index], result, operands[index + 1]);

  printf("%f\n", result);
  return 0;
}
