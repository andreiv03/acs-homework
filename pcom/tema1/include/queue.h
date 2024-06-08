#ifndef _QUEUE_H_
#define _QUEUE_H_

struct queue;
typedef struct queue *queue;

extern queue queue_create(void);

extern void queue_enq(queue q, void *element);

extern void *queue_deq(queue q);

extern int queue_empty(queue q);

#endif
