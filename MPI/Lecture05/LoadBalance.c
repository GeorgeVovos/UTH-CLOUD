#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
#include "math.h"
#define N 31

enum NotificationType
{
    None = 0,
    RequestProcessorPower = 1
};

int main(argc, argv)
int argc;
char **argv;
{
    int rank, worldSize;
    int root = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    int processorPower = pow(2, rank); // Not random, it's a power of 2 so I can easily check the results
    int *processorPowersArrayRaw = NULL;
    int *processorPowersArrayNormalized = NULL;
    float *sourceArray = NULL;
    float *resultArray = NULL;
    if (rank == root)
    {
        processorPowersArrayRaw = (int *)malloc(worldSize * sizeof(int));
        processorPowersArrayNormalized = (int *)malloc(worldSize * sizeof(int));
        sourceArray = (float *)malloc(N * sizeof(float));
        resultArray = (float *)malloc(N * sizeof(float));
        for (int i = 0; i < worldSize; i++)
        {
            processorPowersArrayRaw[i] = 0;
            processorPowersArrayNormalized[i] = 0;
        }

        for (int i = 0; i < N; i++)
        {
            sourceArray[i] = i; // sourceArray[i] = 1;
            resultArray[i] = 0;
        }
    }

    enum NotificationType notificationType = RequestProcessorPower;
    // MPI_Bcast(&notificationType, 1, MPI_INT, root, MPI_COMM_WORLD);

    // if (notificationType == RequestProcessorPower) {
    MPI_Gather(&processorPower, 1, MPI_INT, processorPowersArrayRaw, 1, MPI_INT, root, MPI_COMM_WORLD);
    // }

    if (rank == root)
    {
        printf("Processing powers of all nodes:\n");
        int totalPower = 0;
        int totalNormalizedPower = 0;
        for (int i = 0; i < worldSize; i++)
        {
            printf("Processor power raw - Node %2d: %2d\n", i, processorPowersArrayRaw[i]);
            totalPower += processorPowersArrayRaw[i];
        }

        for (int i = 0; i < worldSize; i++)
        {
            processorPowersArrayNormalized[i] = (int)round((double)processorPowersArrayRaw[i] / totalPower * 100);
            totalNormalizedPower += processorPowersArrayNormalized[i];
            printf("Processor power normalized - Node %2d: %2d\n", i, processorPowersArrayNormalized[i]);
        }
        printf("Total processing power: %d\n", totalPower);
        printf("Total processing power normalized: %d\n", totalNormalizedPower);

        printf("\nSource array ");
        for (int i = 0; i < N; i++)
        {
            printf("%.2f ", sourceArray[i]);
        }
        // printf("\nResult array ");

        // for (int i = 0; i < N; i++) {
        //     printf("%.2f ", resultArray[i]);
        // }
        printf("\n");
        free(processorPowersArrayRaw);
    }

    int *sendCounts = NULL;
    int *displacements = NULL;
    int sendCountsSum = 0;
    if (rank == root)
    {
        sendCounts = (int *)malloc(worldSize * sizeof(int));
        displacements = (int *)malloc(worldSize * sizeof(int));
        int offset = 0;
        for (int i = 0; i < worldSize; i++)
        {
            sendCounts[i] = (int)round((double)processorPowersArrayNormalized[i] / 100 * N);
            displacements[i] = offset;
            offset += sendCounts[i];

            sendCountsSum += sendCounts[i];
        }

        sendCounts[worldSize - 1] = sendCounts[worldSize - 1] + (N - sendCountsSum); // last node gets the rest of the elements
    }

    int receiveCount = 0;
    MPI_Scatter(sendCounts, 1, MPI_INT, &receiveCount, 1, MPI_INT, root, MPI_COMM_WORLD);

    float *receivedArray = (float *)malloc(receiveCount * sizeof(float));
    MPI_Scatterv(sourceArray, sendCounts, displacements, MPI_FLOAT, receivedArray, receiveCount, MPI_FLOAT, root, MPI_COMM_WORLD);

    for (int i = 0; i < receiveCount; i++)
    {
        receivedArray[i] = (float)(receivedArray[i] * 3.14);
    }

    printf("\nNode %d received %d elements ", rank, receiveCount);
    printf("\n");
    MPI_Gatherv(receivedArray, receiveCount, MPI_FLOAT, resultArray, sendCounts, displacements, MPI_FLOAT, root, MPI_COMM_WORLD);

    free(receivedArray);
    if (rank == root)
    {
        free(sendCounts);
        free(displacements);

        printf("\nResult array ");
        for (int i = 0; i < N; i++)
        {
            printf("%.2f ", resultArray[i]);
        }
        printf("\n");
        free(processorPowersArrayNormalized);
        free(sourceArray);
        free(resultArray);
    }

    MPI_Finalize();
    return 0;
}