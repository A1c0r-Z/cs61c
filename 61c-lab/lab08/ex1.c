#include "ex1.h"

void v_add_naive(double* x, double* y, double* z) {
    #pragma omp parallel
    {
        for(int i=0; i<ARRAY_SIZE; i++)
            z[i] = x[i] + y[i];
    }
}

// Adjacent Method
void v_add_optimized_adjacent(double* x, double* y, double* z) {
    // TODO: Implement this function
    // Do NOT use the `for` directive here!
    #pragma omp parallel
    {
        int n = omp_get_num_threads();
        int id = omp_get_thread_num();
        for(int i= id;i<ARRAY_SIZE;i+=n)
        {
            z[i] = x[i] + y[i];
        }
    }
}

// Chunks Method
void v_add_optimized_chunks(double* x, double* y, double* z) {
    // TODO: Implement this function
    // Do NOT use the `for` directive here!
    #pragma omp parallel
    {
        int n = omp_get_num_threads();
        int k = ARRAY_SIZE/n;
        int id = omp_get_thread_num();
        int t = (n==id+1)?ARRAY_SIZE:((id+1)*k);
        for(int i=id*k;i<t;i+=1)
        {
            z[i] = x[i] + y[i];
        }
    }
}
