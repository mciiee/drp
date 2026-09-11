#ifndef QUEUE_H
#define QUEUE_H
#include <stdatomic.h>
#include <stddef.h>

#ifndef QUEUE_SIZE
#define QUEUE_SIZE 2048
#endif

struct QueueEntry {
  void *message;
  size_t length;
};

typedef struct Queue {
  atomic_size_t head;
  atomic_size_t tail;
  // atomic_size_t count;
  struct QueueEntry entries[QUEUE_SIZE];
} Queue;

Queue *Queue_new(void);
int Queue_append(Queue *queue, void *msg, size_t length);
int Queue_pop(Queue *queue, struct QueueEntry *entry);
#endif
