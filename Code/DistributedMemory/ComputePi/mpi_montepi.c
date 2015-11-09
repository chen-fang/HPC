#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

long long Play( long long local_n, int my_rank )
{
   int seed = my_rank + 256;
   srand( seed );
   long long local_count = 0;
   for( long long int toss = 0; toss < local_n; toss++ )
   {
      double x = rand() / (double)RAND_MAX * 2.0 - 1.0;
      double y = rand() / (double)RAND_MAX * 2.0 - 1.0;
      double d = x*x + y*y;
      if( d <= 1.0 )
	 local_count++;
   }

   return local_count;
}

int main( int argc, char* argv[] )
{
   int my_rank;
   int comm_sz;

   MPI_Init( &argc, &argv );
   MPI_Comm_size( MPI_COMM_WORLD, &comm_sz );
   MPI_Comm_rank( MPI_COMM_WORLD, &my_rank );

   long long number_of_tosses = atoll( argv[1] );
   long long local_n = number_of_tosses / (long long)comm_sz;
   
   MPI_Barrier( MPI_COMM_WORLD );
   double time_start = MPI_Wtime();

   long long local_count = Play( local_n, my_rank );

   double time_end = MPI_Wtime();
   double time_elapsed = time_end - time_start;
   double max_time_elapsed = 0.0;
   MPI_Reduce( &time_elapsed, &max_time_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );

   long long total_count = 0;
   MPI_Reduce( &local_count, &total_count, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD );
   MPI_Barrier( MPI_COMM_WORLD );

   if( my_rank == 0 )
   {
      double pi = 4.0 * total_count / (double)number_of_tosses;
      // printf( "%2d |  %lld / %lld  |  %6.5f   ||  %e seconds\n", comm_sz, total_count, number_of_tosses, pi, max_time_elapsed );
   }

   printf( "rank #%d  |  %e seconds\n", my_rank, time_elapsed );

   MPI_Finalize();
   
   return 0;
}
