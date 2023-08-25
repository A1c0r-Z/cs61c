#include <time.h>
#include <stdio.h>
#include <x86intrin.h>
#include "ex1.h"

long long int sum(int vals[NUM_ELEMS]) {
    clock_t start = clock();

    long long int sum = 0;
    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        for(unsigned int i = 0; i < NUM_ELEMS; i++) {
            if(vals[i] >= 128) {
                sum += vals[i];
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return sum;
}

long long int sum_unrolled(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    long long int sum = 0;

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
            if(vals[i] >= 128) sum += vals[i];
            if(vals[i + 1] >= 128) sum += vals[i + 1];
            if(vals[i + 2] >= 128) sum += vals[i + 2];
            if(vals[i + 3] >= 128) sum += vals[i + 3];
        }

        // TAIL CASE, for when NUM_ELEMS isn't a multiple of 4
        // NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
        // Order is important, since (NUM_ELEMS / 4) effectively rounds down first
        for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                sum += vals[i];
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return sum;
}

long long int sum_simd(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    __m128i _127 = _mm_set1_epi32(127); // This is a vector with 127s in it... Why might you need this?
    long long int result = 0; // This is where you should put your final result!
    /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */
    __m128i sum_vec = _mm_setzero_si128();
    int sum[4] = {0};
    long long int ans = 0;
    __m128i tmp;
    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        /* YOUR CODE GOES HERE */
        for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4 ; i += 4) {
            tmp = _mm_loadu_si128((__m128i *) (vals+i));
            tmp = _mm_and_si128(_mm_cmpgt_epi32(tmp, _127),tmp);
            sum_vec = _mm_add_epi32(sum_vec, tmp);
            /* Hint: you'll need a tail case. */
        }
        _mm_storeu_si128((__m128i *)sum,sum_vec);
        ans = ans + sum[0]+sum[1]+sum[2]+sum[3];
        sum[0] = 0;
        sum[1] = 0;
        sum[2] = 0;
        sum[3] = 0;
        sum_vec = _mm_setzero_si128();
        for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                ans += vals[i];
        }
    }
    }
    result = ans;
    /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return result;
}

long long int sum_simd_unrolled(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    __m128i _127 = _mm_set1_epi32(127);
    long long int result = 0;
    /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */
    __m128i sum_vec = _mm_setzero_si128();
    int sum[4] = {0};
    long long int ans = 0;
    __m128i tmp;
    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        /* YOUR CODE GOES HERE */
        for(unsigned int i = 0; i < NUM_ELEMS / 16 * 16; i += 16) {
            tmp = _mm_loadu_si128((__m128i *) (vals+i));
            tmp = _mm_and_si128(_mm_cmpgt_epi32(tmp, _127),tmp);
            sum_vec = _mm_add_epi32(sum_vec, tmp);
            tmp = _mm_loadu_si128((__m128i *) (vals+i+4));
            tmp = _mm_and_si128(_mm_cmpgt_epi32(tmp, _127),tmp);
            sum_vec = _mm_add_epi32(sum_vec, tmp);
            tmp = _mm_loadu_si128((__m128i *) (vals+i+8));
            tmp = _mm_and_si128(_mm_cmpgt_epi32(tmp, _127),tmp);
            sum_vec = _mm_add_epi32(sum_vec, tmp);
            tmp = _mm_loadu_si128((__m128i *) (vals+i+12));
            tmp = _mm_and_si128(_mm_cmpgt_epi32(tmp, _127),tmp);
            sum_vec = _mm_add_epi32(sum_vec, tmp);
            /* Hint: you'll need a tail case. */
        }
        _mm_storeu_si128((__m128i *)sum,sum_vec);
        ans = ans + sum[0]+sum[1]+sum[2]+sum[3];
        sum[0] = 0;
        sum[1] = 0;
        sum[2] = 0;
        sum[3] = 0;
        sum_vec = _mm_setzero_si128();
        for(unsigned int i = NUM_ELEMS / 16 * 16; i < NUM_ELEMS; i++) {
        if (vals[i] >= 128) {
                ans += vals[i];
        }
    }
    }
    result = ans;
    /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return result;
}
