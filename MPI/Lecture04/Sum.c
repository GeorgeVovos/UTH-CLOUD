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

    int *array = (int *)malloc(N * sizeof(int));
    int *arrayAcc = (int *)malloc(N * sizeof(int));
    array[0] = 3;
    array[1] = 1;
    array[2] = 4;
    array[3] = 3;
    array[4] = 1;
    array[5] = 5;
    // S={3,4,8,11,12,17}

    arrayAcc[0] = array[0];

    for (int i = 1; i < N; i++)
    {
        arrayAcc[i] = arrayAcc[i - 1] + array[i];
    }

    for (size_t i = 0; i < N; i++)
    {
        printf("%d ", arrayAcc[i]);
    }


    printf("\n");
    MPI_Finalize();

    return 0;
}