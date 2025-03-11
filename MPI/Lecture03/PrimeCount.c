#include <mpi.h>
#include <stdio.h>

int isPrime(int x) {
    if (x < 2) return 0; 
    for (int d = 2; d <= x / 2; d++) {
        if (x % d == 0) return 0;  
    }
    return 1;  
}

int main(int argc, char** argv) {
    int rank, size, N = 1000000, localCount = 0, globalCount = 0;

    MPI_Init(&argc, &argv);  
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0 ) {
        printf("Calculating the number of primes up to %d\n", N);
    }

    double startTime, endTime;
    startTime = MPI_Wtime(); 

    int start = (rank * N) / size + 1;
    int end = ((rank + 1) * N) / size;

    for (int x = start; x <= end; x++) {
        if (isPrime(x)) {
            localCount++;
        }
    }

    MPI_Reduce(&localCount, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        endTime = MPI_Wtime();
        printf("Total prime numbers up to %d: %d\n", N, globalCount);
        printf("\n\nTime needed:%.6f\n\n", endTime-startTime);
    }

    MPI_Finalize();
    return 0;
}
