#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define UINT_MAX_BITS 32
#define SHORT_MAX_BITS 16
#define LENGTH_BITS 3
#define OPERATOR_BITS 2
#define DIMENSION_BITS 4

int getTotalBits(int number) {
  return (int)log2(number) + 1;
}

int concatenateBinaryNumbers(int a, int b, int padding) {
  return (a << padding) | b;
}

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

void getOperands(int length, int dimension, int16_t *numbers, int *operands) {
  size_t numbersIndex = 0;
  int position = SHORT_MAX_BITS;

  for (size_t operandsIndex = 0; operandsIndex < length + 1; ++operandsIndex) {
    int currentDimension = dimension;
    int mask = 0;
    int operand = 0;
    int leftPart = 0, rightPart = 0;

    if (position < currentDimension) {
      mask = (1 << position) - 1;
      leftPart = numbers[numbersIndex++] & mask;
      currentDimension = dimension - position;
      position = SHORT_MAX_BITS;
    }

    mask = ((1 << currentDimension) - 1) << (position - currentDimension);
    rightPart = (numbers[numbersIndex] & mask) >> (position - currentDimension);
    operand = concatenateBinaryNumbers(leftPart, rightPart, currentDimension);
    operands[operandsIndex] = operand;
    position = position - currentDimension;
  }
}

void solve(int length, char *operators, int *operands) {
  for (int index = 0; index < length; ++index) {
    if (operators[index] == '*')
      operands[index + 1] = operands[index] * operands[index + 1];

    if (operators[index] == '/')
      operands[index + 1] = operands[index] / operands[index + 1];
  }

  for (int index = 0, count = 1; index < length + 1; ++index) {
    if (operators[index] == '+' || operators[index] == '-') {
      while (operators[index + count] == '*' || operators[index + count] == '/')
        count = count + 1;

      if (operators[index] == '+')
        operands[index + count] = operands[index] + operands[index + count];

      if (operators[index] == '-')
        operands[index + count] = operands[index] - operands[index + count];

      count = 1;
    }
  }

  printf("%d\n", operands[length]);
}

int main() {
  unsigned int instruction = 0;
  scanf("%u", &instruction);

  int length = getLength(instruction);

  const size_t OPERATORS_SIZE = length;
  char operators[OPERATORS_SIZE];
  getOperators(instruction, length, operators);

  int dimension = getDimension(instruction, length);

  const size_t NUMBERS_SIZE = ceil(1.0 * ((length + 1) * dimension) / 16);
  int16_t numbers[NUMBERS_SIZE];
  for (size_t index = 0; index < NUMBERS_SIZE; ++index)
    scanf("%hd", numbers + index);

  const size_t OPERANDS_SIZE = length + 1;
  int operands[OPERANDS_SIZE];
  getOperands(length, dimension, numbers, operands);

  solve(length, operators, operands);

  return 0;
}
