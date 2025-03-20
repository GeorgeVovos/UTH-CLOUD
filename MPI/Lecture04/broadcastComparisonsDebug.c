#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

//#define N 5000000  // Large data size for testing
#define N 5 // Small data size for debugging :)

void SumAndPrintArray(char *method, int *data, int size, int rank, int worldSize)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        printf("%s : Rank %d data[%d] %d\n", method, rank, i, data[i]);
        sum += data[i];
    }
    printf("%s Rank %d sum %d\n", method, rank, sum);
}

void BcastBroadcast(int *data, int size, int rank, int worldSize)
{
    // SumAndPrintArray("Before Bcast", data, size,rank,MPI_COMM_WORLD);
    MPI_Bcast(data, size, MPI_INT, 0, MPI_COMM_WORLD);
    // SumAndPrintArray("After Bcast", data, size,rank,MPI_COMM_WORLD);
}

void SimpleSendReceiveLoop(int *data, int size, int rank, int worldSize)
{
    if (rank == 0)
    {
        for (int i = 1; i < worldSize; i++)
        {
            MPI_Send(data, size, MPI_INT, i, 0, MPI_COMM_WORLD);
            //printf("Simple: Rank %d send data to %d \n", rank,i);
        }
    }
    else
    {
        MPI_Recv(data, size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //SumAndPrintArray("After Simple Receive", data, size,rank,MPI_COMM_WORLD);
    }
}

void SmartSendReceiveLoop(int *data, int size, int rank, int worldSize)
{
    int step = 1;
    while (step < worldSize)
    {
        if (rank < step && rank + step < worldSize)
        {
            MPI_Send(data, size, MPI_INT, rank + step, 0, MPI_COMM_WORLD);
            printf("Smart: Rank %d send data to %d \n", rank, rank + step);
        }

        if (rank >= step && rank < step * 2)
        {
            MPI_Recv(data, size, MPI_INT, rank - step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Smart: Rank %d received data\n", rank);

            SumAndPrintArray("After Smart Receive", data, size,rank,MPI_COMM_WORLD);
        }
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
