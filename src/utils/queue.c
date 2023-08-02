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
 * Allocates an empty queue and returns a pointer to it.
 */
queue_t *queue_init(void) {

    int semaphore_status;
    int mutex_status;
    queue_t *queue = (queue_t *) malloc(sizeof(queue));
    
    if (queue == NULL) {
        perror("Memory could not be allocated for queue.");
        exit(EXIT_FAILURE);
    }

    queue->head = NULL;
    queue->tail = NULL;
    
    semaphore_status = sem_init(&(queue->n), false, 0);

    if (semaphore_status == -1) {
        perror("Could not initialise queue semaphore.");
        exit(EXIT_FAILURE);
    }

    mutex_status = pthread_mutex_init(&(queue->lock), NULL);

    if (mutex_status == -1) {
        perror("Could not initalise queue mutex.");
        exit(EXIT_FAILURE);
    }

    return queue;
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

    sem_post(&(queue->n));
}

/**
 * Gets the next item on the queue when it becomes available.
 */
void *queue_get(queue_t *queue) {

    void* data;
    queue_item_t *old_head;
    
    // Wait for items to be on the queue
    sem_wait(&(queue->n));

    // Then get a lock on queue operations
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

    return data;
}

/**
 * Frees a queue and all the items on the queue.
 */
void queue_free(queue_t *queue) {

    queue_item_t *item;

    pthread_mutex_lock(&(queue->lock));

    while (queue->head != NULL) {
        item = queue->head;
        queue->head = item->next;
        free(item);
    }

    free(queue->tail);

    pthread_mutex_destroy(&(queue->lock));
    sem_destroy(&(queue->n));

    free(queue);
}
