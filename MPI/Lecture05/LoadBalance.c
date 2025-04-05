#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
#include "math.h"
#define N 30

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
    //printf("\n rank %d ",rank);
    int proc_power = pow(2,rank);// Each node's processing power
    int *processorPowersArrayRaw = NULL;
    int *processorPowersArrayNormalized= NULL;
    int *sourceArray = NULL;
    int *resultArray = NULL;
    if (rank == root) {
        processorPowersArrayRaw = (int *)malloc(worldSize * sizeof(int));
        processorPowersArrayNormalized = (int *)malloc(worldSize * sizeof(int));
        sourceArray = (int *)malloc(N * sizeof(int));
        resultArray = (int *)malloc(N * sizeof(int));
        for (int i = 0; i < worldSize; i++)
        {
            processorPowersArrayRaw[i] = 0;
            processorPowersArrayNormalized[i] = 0;
        }

        for (int i = 0; i < N; i++)
        {
            sourceArray[i] = 1;
            resultArray[i] = 0;
        }
    }

    enum NotificationType notificationType = RequestProcessorPower;
   // MPI_Bcast(&notificationType, 1, MPI_INT, root, MPI_COMM_WORLD);

    //if (notificationType == RequestProcessorPower) {
        MPI_Gather(&proc_power, 1, MPI_INT, processorPowersArrayRaw, 1, MPI_INT, root, MPI_COMM_WORLD);
   // }

    if (rank == root) {
        printf("Processing powers of all nodes:\n");
        int totalPower = 0;
        int totalNormalizedPower = 0;
        for (int i = 0; i < worldSize; i++) {
            printf("Processor power raw - Node %2d: %2d\n", i, processorPowersArrayRaw[i]);
            totalPower += processorPowersArrayRaw[i];
        }

       
        for (int i = 0; i < worldSize; i++) {
            processorPowersArrayNormalized[i] = (int)round((double)processorPowersArrayRaw[i] / totalPower * 100);
            totalNormalizedPower += processorPowersArrayNormalized[i];  
            printf("Processor power normalized - Node %2d: %2d\n", i, processorPowersArrayNormalized[i]);
            
        }
        printf("Total processing power: %d\n", totalPower);
        printf("Total processing power normalized: %d\n", totalNormalizedPower);
     

        printf("\nSource array ");
        for (int i = 0; i < N; i++) {
            printf("%2d ", sourceArray[i]);
        }
        printf("\nResult array ");

        for (int i = 0; i < N; i++) {
            printf("%2d ", resultArray[i]);
        }
        printf("\n");
        free(processorPowersArrayRaw);
        free(sourceArray);
        free(resultArray);
    }

    MPI_Finalize();

    return 0;
}