#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ARRAY_SIZE 2048
#define NUMBER_OF_TRIALS 100000

/*
 * Statically allocate our arrays.  Compilers can
 * align them correctly.
 */
static double a[ARRAY_SIZE], b[ARRAY_SIZE], c;

int main(int argc, char *argv[]) {
    int i,t;

    double m = 1.0001;

    /* Populate A and B arrays */
    for (i=0; i < ARRAY_SIZE; i++) {
        b[i] = i;
        a[i] = i+1;
    }

    /* Time */
    clock_t time = clock();

    c = 0;

    /* Perform an operation a number of times */
    for (t=0; t < NUMBER_OF_TRIALS; t++) {
        for (i=0; i < ARRAY_SIZE; i++) {
            c += m*a[i] + b[i];
        }
    }

    /* Time */
    time = clock() - time;
    double secs = ((double)time)/CLOCKS_PER_SEC;
    printf("Time in seconds = %f\n", secs);

    printf("c = %f\n", c);

    return 0;
}
