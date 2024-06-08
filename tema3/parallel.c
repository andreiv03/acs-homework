#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "os_graph.h"
#include "os_threadpool.h"
#include "log/log.h"
#include "utils.h"

#define NUM_THREADS 4

static int sum;
static os_graph_t *graph;
static os_threadpool_t *tp;

pthread_mutex_t sum_mutex;
pthread_mutex_t graph_mutex;

static void process_node(void *idx) {
  if (graph->visited[*(unsigned int *) idx] == DONE)
    return;

  os_node_t *node = graph->nodes[*(unsigned int *) idx];

  pthread_mutex_lock(&sum_mutex);

  sum = sum + node->info;
  graph->visited[*(unsigned int *) idx] = DONE;

  for (unsigned int neigh_idx = 0; neigh_idx < node->num_neighbours; ++neigh_idx) {
    pthread_mutex_lock(&graph_mutex);

    if (graph->visited[node->neighbours[neigh_idx]] == NOT_VISITED) {
      graph->visited[node->neighbours[neigh_idx]] = PROCESSING;

      unsigned int *arg = malloc(sizeof(unsigned int));
      DIE(arg == NULL, "malloc");
      *arg = node->neighbours[neigh_idx];

      os_task_t *t = create_task(process_node, arg, free);
      enqueue_task(tp, t);
    }

    pthread_mutex_unlock(&graph_mutex);
  }

  pthread_mutex_unlock(&sum_mutex);
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

  pthread_mutex_init(&sum_mutex, NULL);
  pthread_mutex_init(&graph_mutex, NULL);

  tp = create_threadpool(NUM_THREADS);

  unsigned int *arg = malloc(sizeof(unsigned int));
  DIE(arg == NULL, "malloc");
  *arg = 0;

  os_task_t *t = create_task(process_node, arg, free);
  enqueue_task(tp, t);
  wait_for_completion(tp);
  destroy_threadpool(tp);

  printf("%d", sum);

  pthread_mutex_destroy(&sum_mutex);
  pthread_mutex_destroy(&graph_mutex);

  return 0;
}
