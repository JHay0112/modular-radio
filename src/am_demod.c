/**
 * Demodulates an AM signal using envelop detection.
 * 
 * Floating point numbers formatted in scientific notation are passed 
 * into the channel through stdin, and returned through stdout. 
 * This permits use by piping through the shell e.g.
 * 
 * `$ ./transmitter | ./channel | ./receiver`.
 * 
 * This should encourage some level of modularity at the cost of a slight
 * overhead. Note that this script performs a level of batch operation by
 * handing collections of data to threads for processing.
 * 
 * Author: J. L. Hay
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "utils/queue.h"


#define NUM_WORKERS 3


static pthread_t workers[NUM_WORKERS];

static queue_t in_queue;
static queue_t out_queue;


void worker(void) {

    float *data;

    while (true) {

        data = (float *) queue_get(&in_queue);

        // Pretend this is way more complicated...
        *data = fabs(*data);
        sleep(1);

        queue_add(&out_queue, (void *) data);
    }
}

int main(void) {

    float in;

    // Initialise queues
    queue_init(&in_queue);
    queue_init(&out_queue);

    // Setup worker pool
    for (size_t i = 0; i < NUM_WORKERS; i++) {
        pthread_create(&(workers[i]), NULL, (void *) &worker, NULL);
    }
    
    while (scanf("%f", &in) != EOF) {
        float *data = (float *) malloc(sizeof(float));
        *data = in;
        queue_add(&in_queue, (void *) data);
    }

    while (!queue_empty(&out_queue)) {
        // Get all the data out
        float *data = queue_get(&out_queue);
        printf("%f\n", *data);
        free(data);
    }
}