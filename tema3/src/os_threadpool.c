#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "os_threadpool.h"
#include "log/log.h"
#include "utils.h"

os_task_t *create_task(void (*action)(void *), void *arg, void (*destroy_arg)(void *)) {
  os_task_t *t;

  t = malloc(sizeof(*t));
  DIE(t == NULL, "malloc");

  t->action = action;
  t->argument = arg;
  t->destroy_arg = destroy_arg;

  return t;
}

void destroy_task(os_task_t *t) {
  if (t->destroy_arg != NULL)
    t->destroy_arg(t->argument);
  free(t);
}

void enqueue_task(os_threadpool_t *tp, os_task_t *t) {
  assert(tp != NULL);
  assert(t != NULL);

  pthread_mutex_lock(&tp->mutex);
  list_add_tail(&tp->head, &t->list);
  tp->num_tasks++;
  pthread_mutex_unlock(&tp->mutex);
  pthread_cond_broadcast(&tp->cond);
}

static int queue_is_empty(os_threadpool_t *tp) {
  return list_empty(&tp->head);
}

os_task_t *dequeue_task(os_threadpool_t *tp) {
  os_task_t *t = NULL;

  pthread_mutex_lock(&tp->mutex);

  while (queue_is_empty(tp) && !tp->shutdown) {
    tp->waiting_threads++;
    pthread_cond_wait(&tp->cond, &tp->mutex);
    pthread_mutex_lock(&tp->mutex);
    tp->waiting_threads--;
  }

  if (tp->shutdown) {
    pthread_mutex_unlock(&tp->mutex);
    return NULL;
  }

  if (!queue_is_empty(tp)) {
    os_list_node_t *node = tp->head.next;
    list_del(node);
    t = list_entry(node, os_task_t, list);
    tp->num_tasks--;
  }

  pthread_mutex_unlock(&tp->mutex);

  return t;
}

static void *thread_loop_function(void *arg) {
  os_threadpool_t *tp = (os_threadpool_t *) arg;

  while (1) {
    os_task_t *t;

    t = dequeue_task(tp);
    if (t == NULL)
      break;

    t->action(t->argument);
    destroy_task(t);
  }

  if (tp->waiting_threads == tp->num_threads - 1) {
    tp->shutdown = 1;
    pthread_cond_broadcast(&tp->cond);
  }

  return NULL;
}

void wait_for_completion(os_threadpool_t *tp) {
  pthread_mutex_lock(&tp->mutex);

  while (tp->num_tasks > 0 || tp->waiting_threads < tp->num_threads - 1)
    pthread_cond_wait(&tp->cond, &tp->mutex);

  pthread_mutex_unlock(&tp->mutex);

//  for (unsigned int i = 0; i < tp->num_threads; i++)
//    pthread_join(tp->threads[i], NULL);
}

os_threadpool_t *create_threadpool(unsigned int num_threads) {
  os_threadpool_t *tp = NULL;
  int rc;

  tp = malloc(sizeof(*tp));
  DIE(tp == NULL, "malloc");

  list_init(&tp->head);

  pthread_mutex_init(&tp->mutex, NULL);
  pthread_cond_init(&tp->cond, NULL);

  tp->num_threads = num_threads;
  tp->threads = malloc(num_threads * sizeof(*tp->threads));
  DIE(tp->threads == NULL, "malloc");

  tp->num_tasks = 0;
  tp->shutdown = 0;
  tp->waiting_threads = 0;

  for (unsigned int i = 0; i < num_threads; ++i) {
    rc = pthread_create(&tp->threads[i], NULL, &thread_loop_function, (void *) tp);
    DIE(rc < 0, "pthread_create");
  }

  return tp;
}

void destroy_threadpool(os_threadpool_t *tp) {
//  os_list_node_t *n, *p;

  pthread_mutex_lock(&tp->mutex);
  tp->shutdown = 1;
  pthread_cond_broadcast(&tp->cond);
  pthread_mutex_unlock(&tp->mutex);

  for (unsigned int i = 0; i < tp->num_threads; i++) {
    pthread_join(tp->threads[i], NULL);
  }

  pthread_mutex_destroy(&tp->mutex);
  pthread_cond_destroy(&tp->cond);

  free(tp->threads);
  free(tp);

//  list_for_each_safe(n, p, &tp->head) {
//    list_del(n);
//    destroy_task(list_entry(n, os_task_t, list));
//  }
}
