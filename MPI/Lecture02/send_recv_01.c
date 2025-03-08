#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int size, rank; 
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
     if (rank == 0) { // master node
        int a = 10;
        MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("node: %d, sent: %d\n", rank, a);
        MPI_Recv(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("\n\nnode: %d, received: %d\n", rank, a);
    } else  { //worker node
        int b;
        MPI_Recv(&b, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("\n\nnode: %d, received: %d\n", rank, b);
        b *= 2;
        printf("node: %d, sent: %d\n", rank, b);
        MPI_Send(&b, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();

   return 0;
}
