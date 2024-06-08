#pragma once
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void SolveTask1(void *info, int nr_avioane);
void SolveTask2(void *info, int nr_avioane, int N, char **mat);
void SolveTask3(void *info, int nr_avioane);
void SolveTask4(void *info, int nr_avioane, int nr_obstacole, int *x, int *y, int N);
void SolveTask5(void *info, int nr_avioane, int T, int nr_pct_coord, int *x, int *y, int N);

void *init_info(int nr_avioane);
int *init_vector(int N);
char **init_mat(int N);
void free_mat(char **mat, int N);

void Task1();
void Task2();
void Task3();
void Task4();
void Task5();
