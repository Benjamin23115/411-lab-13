#include <stdio.h>
#include <omp.h>
#include <stdlib.h> 
#include <time.h>


int parallelComputation(int array[], int n) {
    int result = 0;

    #pragma omp parallel
    {
        #pragma omp single
        for (int i = 0; i < n -1; i++) {
            #pragma omp task firstprivate(i)
            {
                int privateResult = array[i];

                #pragma omp atomic
                result += privateResult;
            } 
        }
    }

    #pragma omp taskwait

    return result;
}


int main() {
    srand(time(NULL));
    int array[rand() % 100];
    int n = sizeof(array) / sizeof(array[0]);

    for (int i = 0; i < sizeof(array) / 4; i++){
        array[i] = rand() % 10;
    }

    int finalResult = parallelComputation(array, n);

    printf("Final results: %d\n", finalResult);
    return 0;
}