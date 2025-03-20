#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5000000  

void BcastBroadcast(int *data, int size, int rank, int worldSize)
{
    MPI_Bcast(data, size, MPI_INT, 0, MPI_COMM_WORLD);
}

void SimpleSendReceiveLoop(int *data, int size, int rank, int worldSize)
{
    if (rank == 0)
        for (int i = 1; i < worldSize; i++)
            MPI_Send(data, size, MPI_INT, i, 0, MPI_COMM_WORLD);
    else
        MPI_Recv(data, size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

void SmartSendReceiveLoop(int *data, int size, int rank, int worldSize)
{
    int step = 1;
    while (step < worldSize)
    {
        if (rank < step && rank + step < worldSize)
            MPI_Send(data, size, MPI_INT, rank + step, 0, MPI_COMM_WORLD);
        if (rank >= step && rank < step * 2)
            MPI_Recv(data, size, MPI_INT, rank - step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        step *= 2;
    }
}

int main(int argc, char **argv)
{
    int rank, worldSize;
    int *data = (int *)malloc(N * sizeof(int));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    if (rank == 0)
    {
        for (int i = 0; i < N; i++)
        {
            data[i] = i;
        }
    }

    double start, end;

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();
    BcastBroadcast(data, N, rank, worldSize);
    end = MPI_Wtime();
    if (rank == 0)
    {
        printf("Bcast Broadcast Time         : %f seconds\n", end - start);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();
    SimpleSendReceiveLoop(data, N, rank, worldSize);
    end = MPI_Wtime();
    if (rank == 0)
    {
        printf("Simple Send/Receive loop Time: %f seconds\n", end - start);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();
    SmartSendReceiveLoop(data, N, rank, worldSize);
    end = MPI_Wtime();
    if (rank == 0)
    {
        printf("Smart Send/Receive loop Time : %f seconds\n", end - start);
    }

    free(data);
    MPI_Finalize();
    return 0;
}
