#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <xmmintrin.h>
#include <x86intrin.h>

#define ARRAY_SIZE 2048
#define NUMBER_OF_TRIALS 1000000

/*
 * Statically allocate our arrays.  Compilers can
 * align them correctly.
 */
static double vector_a[ARRAY_SIZE], vector_b[ARRAY_SIZE], c;

int main(int argc, char *argv[]) {
    int i,t;

    double m = 1.0001;

    __m256d mm = {1.0001,1.0001,1.0001,1.0001};
    __m256d sum = {0.0,0.0,0.0,0.0}; // to hold partial sums


    /* Vector initialization */
    __m256d b_ini = {0.0, 1.0, 2.0, 3.0};
    __m256d a_ini = {1.0, 2.0, 3.0,4.0};
    __m256d increment = {4.0,4.0,4.0,4.0};

    /* Populate A and B arrays */
    for (i=0; i < ARRAY_SIZE; i += 4) { 
        _mm256_store_pd(&vector_b[i], b_ini);
        b_ini = _mm256_add_pd(b_ini, increment);
        _mm256_store_pd(&vector_a[i], a_ini);
        a_ini = _mm256_add_pd(a_ini, increment);
    }

    /* Time */
    clock_t time = clock();

    /* Perform an operation a number of times */
    for (t=0; t < NUMBER_OF_TRIALS; t++) {
        for (i=0; i < ARRAY_SIZE; i+=4) {
            //c += m*a[i] + b[i];

            // Load arrays
            __m256d va = _mm256_load_pd(&vector_a[i]);
            __m256d vb = _mm256_load_pd(&vector_b[i]);

            // Compute m*a+b
            __m256d tmp = _mm256_fmadd_pd(mm,va,vb);

            // Accumulate results
            sum = _mm256_add_pd(tmp, sum);
        }
    }

    /* Vector sum */
    //c = 0;
    //for (i=0; i<4; i++){
    //    c += sum[i];
    //}

    // Get sum[2], sum[3]
    __m128d xmm = _mm256_extractf128_pd (sum, 1);

    // Extend to 256 bits: sum[2], sum[3], 0, 0
    __m256d ymm = _mm256_castpd128_pd256(xmm);

    // Perform sum[0]+sum[1], sum[2]+sum[3], sum[2]+sum[3], 0+0
    sum = _mm256_hadd_pd (sum, ymm); 

    // Perform sum[0]+sum[1]+sum[2]+sum[3]…
    sum = _mm256_hadd_pd (sum, sum);
    c = sum[0];

    

    /* Time */
    time = clock() - time;
    double secs = ((double)time)/CLOCKS_PER_SEC;
    printf("Time in seconds = %f\n", secs);

    printf("c = %f\n", c);

    return 0;
}
