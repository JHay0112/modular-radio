/**
 * Provides a thread-safe queue.
 * 
 * Author: J. L. Hay
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "queue.h"


/**
 * Initialises a queue.
 */
void queue_init(queue_t *queue, size_t capacity) {

    int semaphore_status;
    int mutex_status;
    
    semaphore_status = sem_init(&(queue->n_items), false, 0);

    if (semaphore_status == -1) {
        perror("Could not initialise queue semaphore.");
        exit(EXIT_FAILURE);
    }

    semaphore_status = sem_init(&(queue->capacity), false, capacity);

    if (semaphore_status == -1) {
        perror("Could not initialise queue capacity semaphore.");
        exit(EXIT_FAILURE);
    }

    mutex_status = pthread_mutex_init(&(queue->lock), NULL);

    if (mutex_status == -1) {
        perror("Could not initalise queue mutex.");
        exit(EXIT_FAILURE);
    }
}

/**
 * Adds an item to the queue. Blocks if another thread is accessing the queue.
 */
void queue_add(queue_t *queue, void *data) {

    queue_item_t *item = (queue_item_t *) malloc(sizeof(queue_item_t));

    if (item == NULL) {
        perror("Could not allocate room for another item on the queue.");
        exit(EXIT_FAILURE);
    }

    item->next = NULL;
    item->data = data;

    // About to do queue operation...
    pthread_mutex_lock(&(queue->lock));
    sem_wait(&(queue->capacity));

    if (queue->tail == NULL) {
        // Empty tail implies empty queue
        if (queue->head != NULL) {
            // Uh oh! That should be null
            perror("Queue has head but no tail.");
            exit(EXIT_FAILURE);
        }

        queue->tail = item;
        queue->head = item;
    } else {
        // Queue must have items, so add to tail
        queue->tail->next = item;
        queue->tail = item;
    }

    // No longer operating on queue...
    pthread_mutex_unlock(&(queue->lock));
    sem_post(&(queue->n_items));
}

/**
 * Gets the next item on the queue when it becomes available.
 */
void *queue_get(queue_t *queue) {

    void* data;
    queue_item_t *old_head;
    
    // Wait for items to be on the queue
    sem_wait(&(queue->n_items));
    pthread_mutex_lock(&(queue->lock));

    if (queue->head == NULL) {
        // Uh-oh! That shouldn't be empty
        perror("Queue empty when semaphore said otherwise.");
        exit(EXIT_FAILURE);
    }

    old_head = queue->head;
    data = old_head->data;
    queue->head = old_head->next;

    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    // Finished operating on the queue
    pthread_mutex_unlock(&(queue->lock));

    free(old_head);
    sem_post(&(queue->capacity));

    return data;
}

/**
 * Determines if there are any items in the queue.
 * Requires a lock on the queue.
 */
bool queue_empty(queue_t *queue) {

    bool is_empty;

    pthread_mutex_lock(&(queue->lock));
    // Kind of inferring here... should be reliable though :/
    is_empty = queue->head == NULL;
    pthread_mutex_unlock(&(queue->lock));

    return is_empty;
}