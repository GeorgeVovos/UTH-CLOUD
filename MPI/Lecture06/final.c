#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    float *localSource = (float *)malloc(localSize * sizeof(float));
    MPI_Scatterv(sourceArray, sendCounts, displacements, MPI_FLOAT, localSource, localSize, MPI_FLOAT, root, MPI_COMM_WORLD);

    // double localPi = 1.0;
    // for (int i = 0; i < localSize; i++) {
    //     localPi *= (4.0 * localSource[i] * localSource[i]) / (4.0 * localSource[i] * localSource[i] - 1);
    // }


    MPI_Finalize();
    return 0;
}