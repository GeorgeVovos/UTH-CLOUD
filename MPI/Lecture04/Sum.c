#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define N 6
int main(int argc, char **argv)
{
    int rank, worldSize;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    int root = 0;

    int *array = (int *)malloc(N * sizeof(int));
    int *arrayAcc = (int *)malloc(N * sizeof(int));
    array[0] = 3;
    array[1] = 1;
    array[2] = 4;
    array[3] = 3;
    array[4] = 1;
    array[5] = 5;
    // S={3,4,8,11,12,17}

    // S={3,4,8}
    // S={3,4,9}

    // S={3,4}
    // S={4,7}
    // S={1,6}

    double start, end;

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();
    arrayAcc[0] = array[0];

    for (int i = 1; i < N; i++)
    {
        arrayAcc[i] = arrayAcc[i - 1] + array[i];
    }

    end = MPI_Wtime();
    if (rank == 0)
    {
        printf("Serial Time  : %f seconds\n", end - start);
    }



    MPI_Barrier(MPI_COMM_WORLD);
    for (int i = 1; i < N; i++)
    {
        arrayAcc[i] = 0;
    }

    start = MPI_Wtime();

    //Parallel
    // for (size_t i = 0; i < N; i++)
    // {
    //     printf("%d ", arrayAcc[i]);
    // }

    // MPI_Bcast(array, N, MPI_INT, root, MPI_COMM_WORLD);
    
    printf("\n");
    MPI_Finalize();

    return 0;
}