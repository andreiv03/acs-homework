#include "utils.h"

float applyOperation(char operation, float a, float b) {
  switch (operation) {
    case '+':
      return a + b;

    case '-':
      return a - b;

    case '#':
      return (a + b) * (a + b);

    case '*':
      return a * b;

    case '/':
      return a / b;

    default:
      return 0;
  }
}
