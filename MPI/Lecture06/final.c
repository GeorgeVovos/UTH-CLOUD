#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int findSecondLargest(int arr[], int n) {
    int minInt=-999999;
    if (n < 2) {
        return minInt;
    }

    int first = minInt, second = minInt;

    for (int i = 0; i < n; i++) {
        if (arr[i] > first) {
            second = first;
            first = arr[i];
        } else if (arr[i] > second && arr[i] != first) {
            second = arr[i];
        }
    }

    if (second == minInt) {
        return minInt;
    }

    return second;
}

int main(int argc, char **argv)
{
    int root = 0;

    int worldSize, rank;
    int N = 20;
    int sourceArray[] = {41, 467, 334, 500, 169, 724, 478, 358, 962, 464, 705, 145, 281, 827, 961, 491, 995, 942, 827, 436};
    int processorPowersArrayRaw[] = {3, 2, 4, 1};
    int processorPowersArrayNormalized[] = {0, 0, 0, 0};

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

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

        printf("\n");
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
            sendCounts[i] = (N * processorPowersArrayNormalized[i]) / 100;
            displacements[i] = offset;
            offset += sendCounts[i];
            sendCountsSum += sendCounts[i];
        }

        sendCounts[worldSize - 1] = sendCounts[worldSize - 1] + (N - sendCountsSum);
    }

    int localSize;
    MPI_Scatter(sendCounts, 1, MPI_INT, &localSize, 1, MPI_INT, root, MPI_COMM_WORLD);
    printf("Local size for rank %d: %d\n", rank, localSize);
    int *localSource = (int *)malloc(localSize * sizeof(int));
    MPI_Scatterv(sourceArray, sendCounts, displacements, MPI_INT, localSource, localSize, MPI_INT, root, MPI_COMM_WORLD);

    // double localPi = 1.0;
    // for (int i = 0; i < localSize; i++) {
    //     localPi *= (4.0 * localSource[i] * localSource[i]) / (4.0 * localSource[i] * localSource[i] - 1);
    // }

    // printf("Local source for rank %d: ", rank);
    // for (int i = 0; i < localSize; i++)
    // {
    //     printf(" %d ", localSource[i]);
    // }
    printf("\n");


    int secondLargest = localSource[0];
    for (int i = 1; i < localSize; i++)
    {
        if (localSource[i] > secondLargest)
        {
            secondLargest = localSource[i];
        }
    }
    for (int i = 0; i < localSize; i++)
    {
        if (localSource[i] > secondLargest)
        {
            secondLargest = localSource[i];
        }
    }
    
    //printf("Second largest number in local source for rank %d: %d\n", rank, secondLargest);

    int *secondLargestArray = NULL;
    if (rank == root)
    {
        secondLargestArray = (int *)malloc(worldSize * sizeof(int));
    }
    MPI_Gather(&secondLargest, 1, MPI_INT, secondLargestArray, 1, MPI_INT, root, MPI_COMM_WORLD);
    if (rank == root)
    {
        // printf("Second largest number in local source\n");
        // for (int i = 0; i < worldSize; i++)
        // {
        //     printf(" %d ", secondLargestArray[i]);
        // }
        int secondLargestGlobal = findSecondLargest(secondLargestArray, worldSize);
    
        printf("Second largest number in initial array: %d\n", secondLargestGlobal);
    }

    MPI_Finalize();
    return 0;
}