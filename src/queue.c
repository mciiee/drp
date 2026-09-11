#include "queue.h"
#include <stdatomic.h>
#include <stdlib.h>


Queue *Queue_new(void) {
  Queue *queue = malloc(sizeof(Queue));
  if (queue == nullptr) {
    return nullptr;
  }
  atomic_store_explicit(&queue->head, 0, memory_order_relaxed);
  atomic_store_explicit(&queue->tail, 0, memory_order_relaxed);
  return queue;
}

int Queue_append(Queue *queue, void *msg, size_t length) {
  size_t head = atomic_load(&queue->head);
  size_t tail = atomic_load(&queue->tail);
  atomic_thread_fence(memory_order_acquire);

  if (tail - head >= QUEUE_SIZE) {
    return -1;
  }

  size_t index = tail % QUEUE_SIZE;
  queue->entries[index].message = msg;
  queue->entries[index].length = length;

  atomic_thread_fence(memory_order_release);
  atomic_store_explicit(&queue->tail, tail+1, memory_order_relaxed);
  return 0;
}

int Queue_pop(Queue *queue, struct QueueEntry *entry) {
  size_t head = atomic_load_explicit(&queue->head, memory_order_relaxed);
  size_t tail = atomic_load_explicit(&queue->tail, memory_order_acquire);

  if (head == tail) {
    return -1;
  }
 
  size_t index = head % QUEUE_SIZE;
  
  entry->message = queue->entries[index].message;
  entry->length = queue->entries[index].length;

  atomic_store_explicit(&queue->head, head+1, memory_order_release);
  return 0;
}
