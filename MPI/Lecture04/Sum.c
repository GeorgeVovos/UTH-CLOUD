#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define N 120000000
int main(int argc, char **argv)
{
    int rank, worldSize;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    int root = 0;

    int *array = (int *)malloc(N * sizeof(int));
    int *arrayAcc = (int *)malloc(N * sizeof(int));
    if (rank == 0)
    {
        for (int i = 0; i < N; i++)
        {
            array[i] = 1;
        }
    }

    // S={3,4,8,11,12,17 ,23,30,38,47 ,49 ,52}

    // // S={3,4,8 ,11,12 ,17}
    // // S={6,7,8 ,9 ,2, 3}

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

   

    // Parallel
    start = MPI_Wtime();
    MPI_Bcast(array, N, MPI_INT, root, MPI_COMM_WORLD);
    
    int part;
    // part = (N / worldSize); // εάν N=11, size=3, part=3
    part = N / worldSize + (N % worldSize != 0);
    // if (rank == 0)
    // {
    //     printf("part=%d\n", part);
    // }

    int *arrayLocal = (int *)malloc(part * sizeof(int));
    int *arrayLocalAcc = (int *)malloc(part * sizeof(int));
    int localCounter = 0;
    for (int i = rank * part; i < (rank + 1) * part; i++)
    {
        if (i < N)
        {
            // arrayLocal[localCounter] = arrayLocal[localCounter - 1] + array[i];
            arrayLocal[localCounter] = array[i];
        }
        else
        {
            arrayLocal[localCounter] = 0;
        }

        arrayLocalAcc[i % part] = i % part == 0 ? array[i] : arrayLocalAcc[i % part - 1] + array[i];

       // printf("Rank[%d] arrayLocal[%d]= %d arrayLocalAcc[%d]= %d \n", rank, i % part, arrayLocal[i % part], i % part, arrayLocalAcc[i % part]);

        localCounter++;
    }

    int received = 0;

    if (rank > 0)
    {
       // printf("Rank[%d] receiving= %d\n", rank, received);
        MPI_Recv(&received, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       // printf("Rank[%d] received= %d\n", rank, received);

        for (int j = 0; j < part; j++)
        {
            arrayLocalAcc[j] = arrayLocalAcc[j ] + received;
           // printf("Rank[%d] arrayLocalAcc after Receive[%d]= %d\n", rank, j, arrayLocalAcc[j]);
        }
        if (rank < worldSize - 1)
        {
            // for (int j = 0; j < part; j++)
            // {
            //     printf("            Rank[%d] arrayLocalAcc2 after Receive[%d]= %d\n", rank, j, arrayLocalAcc[j]);
            // }
           // printf("Rank[%d] sent %d\n", rank,arrayLocalAcc[part -  1]);
              MPI_Send(&arrayLocalAcc[part - 1], 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }
    }

    if (rank == 0 )
    {
        //printf("Rank[%d] sending = %d\n", rank, arrayLocalAcc[part - 1]);
        MPI_Send(&arrayLocalAcc[part - 1], 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        // printf("Rank[%d] sent %d\n", rank,arrayLocalAcc[part - 1]);

        // for (int j = 0; j < part; j++)
        //     printf("Rank[%d] arrayLocalAcc [%d]= %d\n", rank, j, arrayLocalAcc[j]);
        
    }

    end = MPI_Wtime();
    if (rank == 0)
    {
        printf("Parallel Time  : %f seconds\n", end - start);
    }

    printf("\n");
    MPI_Finalize();

    return 0;
}