#include <stdio.h>

#include "./src/tasks/task-one.h"
#include "./src/tasks/task-two.h"

int main(void)
{
	FILE *task_one_input_file = fopen("data/urls/urls.in", "r");
	FILE *task_two_input_file = fopen("data/traffic/traffic.in", "r");
	FILE *task_one_output_file = fopen("urls-predictions.out", "w");
	FILE *task_two_output_file = fopen("traffic-predictions.out", "w");

	solve_task_one(task_one_input_file, task_one_output_file);
	solve_task_two(task_two_input_file, task_two_output_file);

	fclose(task_one_input_file);
	fclose(task_two_input_file);
	fclose(task_one_output_file);
	fclose(task_two_output_file);

	return 0;
}
