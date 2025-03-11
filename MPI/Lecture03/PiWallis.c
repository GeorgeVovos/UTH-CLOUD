#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char **argv)
{
    int rank, size; // global to all nodes
    int N, part;
    int i;
    double pi = 1.0, Pi = 1.0;
    double ta, tt;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank == 0 ) { //master node
        printf("\nInput N:");
        fflush(stdout);
        scanf("%d", &N);
   }
   ta = MPI_Wtime(); // start of time measure
   MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD); 
   
   part = N / size; // εάν N=11, size=3, part=3
   for (i=rank*part+1; i<=(rank+1)*part; i++) 
       pi *= (4.0*i*i)/(4.0*i*i-1);
   
   MPI_Reduce(&pi, &Pi, 1, MPI_DOUBLE, MPI_PROD, 0, MPI_COMM_WORLD);
   if ( rank == 0 ) {
       for (i=(N/size)*size+1; i<=N; i++) 
               Pi *= (4.0*i*i)/(4.0*i*i-1);
       Pi *= 2;
       tt = MPI_Wtime(); // end of time
       printf("\n\nPi=%.8f", Pi);
       printf("\nReal pi = %.8f", M_PI);
       printf("\n\nTime needed:%.6f\n\n", tt-ta);
       
   }
   MPI_Finalize();
   return 0;
}
