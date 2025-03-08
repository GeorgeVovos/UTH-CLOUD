#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);  // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Get process rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Get total number of processes

    if (size < 2) {
        printf("This program requires at least two processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0) {
        int number = 5;  // Number to send
        printf("Process 0: Sending %d to Process 1\n", number);
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); // Send number to process 1

        MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receive result
        printf("Process 0: Received %d from Process 1\n", number);
    } else if (rank == 1) {
        int number;
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receive from process 0
        printf("Process 1: Received %d, doubling it\n", number);
        number *= 2;  // Double the number
        MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // Send back to process 0
    }

    MPI_Finalize();  // Finalize MPI
    return 0;
}
