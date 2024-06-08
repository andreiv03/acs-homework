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

  for (int index = 0; index < N - 1; ++index)
    if (operations[index] == '*' || operations[index] == '/')
      operands[index + 1] = applyOperation(operations[index], operands[index], operands[index + 1]);

  for (int index = 0, counter = 1; index < N; ++index) {
    if (operations[index] == '#') {
      while (operations[index + counter] == '*' || operations[index + counter] == '/')
        counter = counter + 1;

      operands[index + counter] = applyOperation(operations[index], operands[index], operands[index + counter]);
      counter = 1;
    }
  }

  for (int index = 0, counter = 1; index < N; ++index) {
    if (operations[index] == '+' || operations[index] == '-') {
      while (operations[index + counter] == '#' ||
             operations[index + counter] == '*' ||
             operations[index + counter] == '/')
        counter = counter + 1;

      operands[index + counter] = applyOperation(operations[index], operands[index], operands[index + counter]);
      counter = 1;
    }
  }

  printf("%f\n", operands[N - 1]);
  return 0;
}
