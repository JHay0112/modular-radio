#ifndef MOD_RADIO_UTILS_QUEUE
#define MOD_RADIO_UTILS_QUEUE

/**
 * Provides a thread-safe queue.
 * 
 * Author: J. L. Hay
 */


#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct queue_item_s {
    struct queue_item_s *next;
    void *data;
} queue_item_t;

typedef struct {
    queue_item_t *head;
    queue_item_t *tail;
    sem_t capacity;
    sem_t n_items;
    pthread_mutex_t change_lock;
} queue_t;


void queue_init(queue_t *queue, size_t capacity);
void queue_add(queue_t *queue, void *data);
void *queue_get(queue_t *queue);
bool queue_empty(queue_t *queue);

#endif