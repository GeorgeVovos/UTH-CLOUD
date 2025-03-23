#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define N 12
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
    array[6] = 6;
    array[7] = 7;
    array[8] = 8;
    array[9] = 9;
    array[10] = 2;
    array[11] = 3;

    // S={3,4,8,11,12,17 ,23,30,38,47 ,49 ,52}

    // // S={3,4,8 ,11,12 ,17}
    // // S={6,7,8 ,9 ,2, 3}

    // // S={3,4,8}
    // // S={1,7}
    // // S={1,6}

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

    // Parallel

    MPI_Bcast(array, N, MPI_INT, root, MPI_COMM_WORLD);
    int part;
    // part = (N / worldSize); // εάν N=11, size=3, part=3
    part = N / worldSize + (N % worldSize != 0);
    if (rank == 0)
    {
        printf("part=%d\n", part);
    }

    int *arrayLocal = (int *)malloc(part * sizeof(int));
    int *arrayLocalAcc = (int *)malloc(part * sizeof(int));
    int localCounter = 0;
    for (int i = rank * part; i < (rank + 1) * part; i++)
    {
        if (i < N)
        {
            // arrayLocal[localCounter] = arrayLocal[localCounter - 1] + array[i];
            arrayLocal[localCounter] =  array[i];
        }
        else
        {
            arrayLocal[localCounter] = 0;
        }

        
        arrayLocalAcc[i%part] = i%part == 0 ?  array[i] :  arrayLocalAcc[i%part -1]  +array[i];
        //arrayLocalAcc[localCounter] =array[i-1] +array[i];

        printf("Rank[%d] arrayLocal[%d]= %d arrayLocalAcc[%d]= %d \n", rank, i%part, arrayLocal[i%part] , i%part, arrayLocalAcc[i%part]);

      
        // if (i <= rank)
        // {
        //      for (int j = 1; j < part; j++)
        //      {
        //          arrayLocalAcc[j] = arrayLocalAcc[j - 1] + arrayLocal[j];
        //          printf("Rank[%d] arrayLocalAcc[%d]= %d\n", rank, j, arrayLocalAcc[j]);
        //      }
        // }

        localCounter++;
    }

    // for (size_t i = 0; i < N; i++)
    // {
    //     printf("%d ", arrayAcc[i]);
    // }

    // MPI_Bcast(array, N, MPI_INT, root, MPI_COMM_WORLD);

    printf("\n");
    MPI_Finalize();

    return 0;
}