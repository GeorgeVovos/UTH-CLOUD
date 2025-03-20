#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#define N 5

int main(argc, argv)
int argc;
char **argv;
{
    int rank;

    int *value = (int *)malloc(N * sizeof(int));
    int root = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        for (int i = 0; i < N; i++)
        {
            value[i] = i;
        }
    }

    printf("process %d: Before MPI_Bcast, value is %d\n", rank, value[1]);

    MPI_Bcast(value, N, MPI_INT, root, MPI_COMM_WORLD);
    
    printf("process %d: After MPI_Bcast, value is %d\n", rank, value[1]);

    MPI_Finalize();

    return 0;
}