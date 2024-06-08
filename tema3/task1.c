#include <math.h>
#include <stdio.h>

#define UINT_MAX_BITS 32
#define LENGTH_BITS 3
#define OPERATOR_BITS 2
#define DIMENSION_BITS 4

int getLength(unsigned int instruction) {
  return (int)(instruction >> (UINT_MAX_BITS - LENGTH_BITS)) + 1;
}

void getOperators(unsigned int instruction, int length, char *operators) {
  for (int index = 0; index < length; ++index) {
    int padding = UINT_MAX_BITS;
    padding = padding - LENGTH_BITS;
    padding = padding - OPERATOR_BITS * (index + 1);

    int code = (int)(instruction >> padding);
    int mask = (1 << OPERATOR_BITS) - 1;
    code = code & mask;

    if (code == 0) operators[index] = '+';
    if (code == 1) operators[index] = '-';
    if (code == 2) operators[index] = '*';
    if (code == 3) operators[index] = '/';
  }
}

int getDimension(unsigned int instruction, int length) {
  int padding = UINT_MAX_BITS;
  padding = padding - LENGTH_BITS;
  padding = padding - OPERATOR_BITS * length;
  padding = padding - DIMENSION_BITS;

  int dimension = (int)(instruction >> padding);
  int mask = (1 << DIMENSION_BITS) - 1;
  return (dimension & mask) + 1;
}

int main() {
  unsigned int instruction = 0;
  scanf("%u", &instruction);

  int length = getLength(instruction);

  const size_t OPERATORS_SIZE = length;
  char operators[OPERATORS_SIZE];
  getOperators(instruction, length, operators);

  int dimension = getDimension(instruction, length);

  printf("%d ", length);
  for (int index = 0; index < length; ++index)
    printf("%c ", operators[index]);
  printf("%d\n", dimension);

  return 0;
}
