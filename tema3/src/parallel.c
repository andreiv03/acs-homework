#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#include "os_graph.h"
#include "os_threadpool.h"
#include "log/log.h"
#include "utils.h"

#define NUM_THREADS 4

static int sum;
static os_graph_t *graph;
static os_threadpool_t *tp;

static pthread_mutex_t sum_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t graph_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t graph_cond = PTHREAD_COND_INITIALIZER;

typedef struct {
  unsigned int idx;
} graph_task_arg_t;

static void process_node(unsigned int idx);

static void process_node_task(void *arg) {
  graph_task_arg_t *task_arg = (graph_task_arg_t *) arg;
  process_node(task_arg->idx);
  free(task_arg);
}

static void process_node(unsigned int idx) {
  os_node_t *node;

  pthread_mutex_lock(&graph_mutex);
  while (graph->visited[idx] != NOT_VISITED) {
    pthread_cond_wait(&graph_cond, &graph_mutex);
  }
  node = graph->nodes[idx];
  graph->visited[idx] = DONE;
  pthread_mutex_unlock(&graph_mutex);
  pthread_cond_broadcast(&graph_cond);

  pthread_mutex_lock(&sum_mutex);
  sum += node->info;
  pthread_mutex_unlock(&sum_mutex);

  for (unsigned int i = 0; i < node->num_neighbours; i++) {
    pthread_mutex_lock(&graph_mutex);
    if (graph->visited[node->neighbours[i]] == NOT_VISITED) {
      pthread_mutex_unlock(&graph_mutex);
      graph_task_arg_t *arg = malloc(sizeof(graph_task_arg_t));
      arg->idx = node->neighbours[i];
      os_task_t *task = create_task((void (*)(void *)) process_node_task, arg, free);
      enqueue_task(tp, task);
    } else {
      pthread_mutex_unlock(&graph_mutex);
    }
  }
}

int main(int argc, char *argv[]) {
  FILE *input_file;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s input_file\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  input_file = fopen(argv[1], "r");
  DIE(input_file == NULL, "fopen");

  graph = create_graph_from_file(input_file);

  tp = create_threadpool(NUM_THREADS);

  graph_task_arg_t *arg = malloc(sizeof(graph_task_arg_t));
  arg->idx = 0;
  os_task_t *task = create_task((void (*)(void *)) process_node, arg, free);
  enqueue_task(tp, task);

  wait_for_completion(tp);
  destroy_threadpool(tp);

  printf("%d", sum);

  return 0;
}
