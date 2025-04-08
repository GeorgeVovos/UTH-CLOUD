#include <mpi.h>
#include <stdio.h>


int main(int argc, char **argv) {
     int size, rank;
     int n=20;

     MPI_Init(&argc, &argv);
     MPI_Comm_size(MPI_COMM_WORLD, &size);
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);

     if (rank == 0) {
           int a[]={41, 467, 334, 500, 169, 724, 478, 358, 962, 464, 705, 145, 281, 827, 961, 491, 995, 942, 827, 436};
           int proc_pow[] = {3, 2, 4, 1};
     }

     MPI_Finalize();
     return 0;
