#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size, number;

    MPI_Init(&argc, &argv);  // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Get process rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Get total number of processes

    if (size < 2) {
        printf("This program requires at least two processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0) {
        number = 1;  // Initial number
        printf("Node %d: Sending %d to Node 1\n", rank, number);
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        
        MPI_Recv(&number, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Node %d: Received %d from Node %d\n", rank, number, size - 1);
    } else {
        MPI_Recv(&number, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        number += 10;
        printf("Node %d: Received %d, increasing to %d and sending to Node %d\n", rank, number - 10, number, (rank + 1) % size);
        
        MPI_Send(&number, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();  // Finalize MPI
    return 0;
}
