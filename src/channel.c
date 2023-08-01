/**
 * Simulates a channel with zero-mean Gaussian noise.
 * 
 * Floating point numbers formatted in scientific notation are passed 
 * into the channel through stdin, and returned through stdout. 
 * This permits use by piping through the shell e.g.
 * 
 * `$ ./transmitter | ./channel | ./receiver`.
 * 
 * This should encourage some level of modularity at the cost of a slight
 * overhead.
 * 
 * Author: J. L. Hay
 */


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>


static const unsigned int NOISE_SEED = 123123123;
static const float NOISE_MEAN = 0.00;
static const float NOISE_STDEV = 1.00;

static const float PI = 3.14159;


/**
 * Produces a zero-mean Gaussian noise term using the Box-Muller method
 * and returns it.
 * 
 * Adapted from https://en.wikipedia.org/wiki/Box-Muller_transform
 */
float noise(void) {

    // Get random integer numbers and scale to floats in [0, 1]
    float num1 = ((float) rand())/((float) RAND_MAX);
    float num2 = ((float) rand())/((float) RAND_MAX);

    // Apply Box-Muller method
    float magnitude = NOISE_STDEV * sqrtf(-2.0 * logf(num1));
    float result = magnitude * cosf(2 * PI * num2) + NOISE_MEAN;

    return result;
}


int main(void) {   

    // Store incoming number
    float in;

    // Initialise random number generator
    srand(NOISE_SEED);

    while (scanf("%f", &in) != EOF) {
        // Add noise to incoming number and print back out
        printf("%f\n", in + noise());
    }
}