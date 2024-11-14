#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int num_threads = 0;

// Function to merge two subarrays
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    int L[n1], R[n2];

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] < R[j]) { // No need to handle equality since values are unique
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSortParallel(int arr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;

        // Divide array into two subarrays
        #pragma omp task shared(arr)
        mergeSortParallel(arr, left, mid);

        #pragma omp task shared(arr)
        mergeSortParallel(arr, mid + 1, right);

        #pragma omp taskwait
        merge(arr, left, mid, right);
    }
}

void mergeSort(int arr[], int left, int right) {
    if (num_threads > 0) {
        omp_set_num_threads(num_threads);
    }

    #pragma omp parallel
    {
        #pragma omp single
        mergeSortParallel(arr, left, right);
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        num_threads = atoi(argv[1]);
        printf("Number of threads: %d\n", num_threads);
    }

    int arr[] = {34, 7, 23, 32, 5, 62};
    int n = sizeof(arr) / sizeof(arr[0]);

    double start_time = omp_get_wtime();

    mergeSort(arr, 0, n-1);

    printf("Sorted array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    double end_time = omp_get_wtime();
    printf("Time: %f seconds\n", end_time - start_time);

    return 0;
}
