#include <omp.h>
#include <stdio.h>
#include <stdlib.h> 
// Function to swap elements
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function for Quicksort
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}
// Parallel Quicksort function
void parallelQuicksort(int arr[], int low, int high, int depth) {
    if (low < high) {
        int pivot = partition(arr, low, high);

        if (depth > 0) { // Limit depth to control task creation overhead
            #pragma omp task shared(arr) if (depth > 0)
            parallelQuicksort(arr, low, pivot - 1, depth - 1);

            #pragma omp task shared(arr) if (depth > 0)
            parallelQuicksort(arr, pivot + 1, high, depth - 1);
        } else { // Sequential if depth threshold is reached
            parallelQuicksort(arr, low, pivot - 1, depth);
            parallelQuicksort(arr, pivot + 1, high, depth);
        }

        #pragma omp taskwait // Wait for parallel tasks to finish
    }
}

// Wrapper function to start parallel execution
void quicksort(int arr[], int n) {
    #pragma omp parallel
    {
        #pragma omp single // Start with a single thread creating initial tasks
        parallelQuicksort(arr, 0, n - 1, 4); // Adjust depth as needed
    }
}

int main() {
    int arr[18];
    int n = sizeof(arr) / sizeof(arr[0]);

    for (int i = 0; i < sizeof(arr) / 4; i++){
        arr[i] = rand() % 100;
    }

double startTime = omp_get_wtime();
    quicksort(arr, n);

    printf("Sorted array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

double endTime = omp_get_wtime();

printf("total time %f", endTime - startTime);
printf("\n");
    return 0;
}