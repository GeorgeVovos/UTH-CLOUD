#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char **argv)
{
    int rank, worldSize;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    // Βήμα 0: 0 -> 1
    // Βήμα 1: 0 -> 2, 1 -> 3
    // Βήμα 2: 0 -> 4, 1 -> 5, 2 -> 6, 3 -> 7
    // Βήμα 3: 0 -> 8, 1 -> 9, 2 -> 10, 3 -> 11, 4 -> 12, 5 -> 13, 6 -> 14, 7 -> 15

    int currentStep = 1;
    while (currentStep < worldSize)
    {
        if (rank < currentStep && rank + currentStep < worldSize)
        {
            printf("%d Sending to %d\n", rank, rank + currentStep);
        }

        // WRONG WHEN TESTING THE REAL THING, WORKERS HAVE NOT RECEIVED THE DATA YET
        // if (rank < currentStep && rank + currentStep < worldSize )
        // {
        //     printf("%d Receiving from %d\n", rank + currentStep, rank );
        // }

        //(PROBABLY) :) CORRECT AS IT PRODUCES THE SAME RESULTS AS THE ABOVE
        if (rank >= currentStep && rank < currentStep * 2)
        {
            printf("%d  Receiving from %d\n",  rank, rank - currentStep );
        }

        currentStep *= 2;
    }

    MPI_Finalize();

    return 0;
}