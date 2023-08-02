#ifndef MOD_RADIO_UTILS_QUEUE
#define MOD_RADIO_UTILS_QUEUE

/**
 * Provides a thread-safe queue.
 * 
 * Author: J. L. Hay
 */


#include <pthread.h>
#include <semaphore.h>


typedef struct {
    queue_item_t *next;
    void *data;
} queue_item_t;

typedef struct {
    queue_item_t *head;
    queue_item_t *tail;
    sem_t n;
    pthread_mutex_t lock;
} queue_t;


queue_t *queue_init(void);
void queue_add(queue_t *queue, void *data);
void *queue_get(queue_t *queue);
void queue_free(queue_t *queue);

#endif