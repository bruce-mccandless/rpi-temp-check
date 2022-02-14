#include <iostream>
#include <cstdlib>
#include <time.h>
#include <pthread.h>

#define NUM_THREADS 4

using namespace std;

struct primes_data {
    int thread_id = 0;
    int total = 0;
};

void *ComputePrimes(void *t_arg) {
    int i, number = 1, total_primes = 0, batch = 10000000;
    struct primes_data *struct_ptr = (struct primes_data*) t_arg;

    while (number <= batch) { 
        i = 2; 
        while (i <= number) { 
            if(number % i == 0)
                break;
            i++; 
        }
        if (i == number) {
            total_primes++;
            struct_ptr->total = total_primes; 
        }
        number++;
    }

    pthread_exit(NULL);
}

int main() {
    clock_t start_t, end_t;
    double total_t;
    int i, total_primes, rc;
    struct primes_data pd[NUM_THREADS];

    pthread_t threads[NUM_THREADS];
    pthread_attr_t a;
    void *status;

    pthread_attr_init(&a);
    pthread_attr_setdetachstate(&a, PTHREAD_CREATE_JOINABLE);

    start_t = clock();

    for (i = 0; i < NUM_THREADS; i++) {
        cout << "main(): creatng thread -> " << i << endl;
        pd[i].thread_id = i;
        pd[i].total = i;
        rc = pthread_create(&threads[i], &a, ComputePrimes, (void *)&pd[i]);
        if (rc) {
            cout << "Error: thread cannot be created, " << rc << endl;
            exit(-1);
        }
    }

    pthread_attr_destroy(&a);
    for (i = 0; i < NUM_THREADS; i++) {
        rc = pthread_join(threads[i], &status);
        if (rc) {
            cout << "Error: thread cannot be joined " << rc << endl;
            exit(-1);
        }
        cout << "Main completed thread id " << i << endl;
        cout << "Status " << status << endl;
    }

    end_t = clock();
    total_t = (end_t - start_t) / (double) CLOCKS_PER_SEC;
    cout << pd->total << " prime numbers found in " << total_t << " seconds" << endl;

    pthread_exit(NULL);
}
