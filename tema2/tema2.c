#include "./utils.h"

#define ONE 1
#define TWO 2
#define THREE 3
#define FOUR 4
#define FIVE 5

// DO NOT MODIFY
// ###########
int main() {
  int task_number = 0;
  scanf("%d\n", &task_number);

  switch (task_number) {
    case ONE:
      Task1();
      break;
    case TWO:
      Task2();
      break;
    case THREE:
      Task3();
      break;
    case FOUR:
      Task4();
      break;
    case FIVE:
      Task5();
      break;
    default:
      printf("Undefined task number.\n");
      break;
  }

  return 0;
}
// ###########
