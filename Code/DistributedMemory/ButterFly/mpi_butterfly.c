#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

double Compute_Partial_Sum( int my_rank, int comm_sz )
{
   double* array;
   long long partial_n;
   double partial_sum = 0.0;
   
   if( my_rank == 0 )
   {
      char word[ 10 ];

      FILE *fp;
      fp = fopen( "gs_data", "r" );
      fscanf( fp, "%s", word );

      long long total_n = atoll( word );

      array = malloc( total_n * sizeof(double) );
      for( long long i = 0; i < total_n; i++ )
      {
	 fscanf( fp, "%s", word );
	 array[i] = atof( word );
      }

      fclose( fp );
      
      printf( "Total N = %lld\n", total_n );

      // --------------------------------------
      // Send data-----------------------------

      partial_n = total_n / comm_sz;
      for( int proc = 1; proc < comm_sz; proc++ )
      {
	 long long send_a = proc * partial_n;
	 MPI_Send( &partial_n,       1,         MPI_LONG_LONG, proc, 0, MPI_COMM_WORLD );
	 MPI_Send( &array[ send_a ], partial_n, MPI_DOUBLE   , proc, 1, MPI_COMM_WORLD );
      }

      // --------------------------------------
      // Compute local sum --------------------

      for( long long i = 0; i < partial_n; i++ )
      {
	 partial_sum += array[i];
      }
      
      free( array );
   }


   if( my_rank != 0 )
   {
      // --------------------------------------
      // Receive partial array ----------------

      MPI_Recv( &partial_n, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );

      double* sub_array = malloc( partial_n * sizeof(double) );
      MPI_Recv( sub_array, partial_n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE );

      // --------------------------------------
      // Compute local sum---------------------

      for( long long i = 0; i < partial_n; i++ )
      {
	 partial_sum += sub_array[i];
      }

      free( sub_array );
   }

   return partial_sum;
}

double Compute_Sum_Butterfly( int my_rank, int comm_sz, double partial_sum )
{
   double sum = partial_sum;
   
   int depth = (int)log2( (double)comm_sz );
   int half_stride = 1;
   int stride = 2 * half_stride;

   int paired_proc;
   
   for( int i = 0; i < depth; i++ )
   {
      if( my_rank % stride < half_stride )
	 paired_proc = my_rank + half_stride;
      else
	 paired_proc = my_rank - half_stride;

      MPI_Send( &sum, 1, MPI_DOUBLE, paired_proc, 0, MPI_COMM_WORLD );

      double recv_partial_sum;
      MPI_Recv( &recv_partial_sum, 1, MPI_DOUBLE, paired_proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );

      /* printf( "my_rank #%d  |  recv_rank #%d  |  receive %f\n", my_rank, paired_proc, recv_partial_sum ); */
      sum += recv_partial_sum;

      half_stride *= 2;
      stride *= 2;
   }
   return sum;
}

int main( int argc, char* argv[] )
{
   int my_rank;
   int comm_sz;

   MPI_Init( &argc, &argv );
   MPI_Comm_size( MPI_COMM_WORLD, &comm_sz );
   MPI_Comm_rank( MPI_COMM_WORLD, &my_rank );

   
   double time_start, time_end, time_elapsed;
   double total_time_elapsed = 0.0;
   MPI_Barrier( MPI_COMM_WORLD );
   time_start = MPI_Wtime();

   
   double partial_sum = Compute_Partial_Sum( my_rank, comm_sz );
   double sum = Compute_Sum_Butterfly( my_rank, comm_sz, partial_sum );
   printf( "rank %2d / %d |  partial_sum = %f  |  sum = %f\n", my_rank, comm_sz, partial_sum, sum );

   
   time_end = MPI_Wtime();
   time_elapsed = time_end - time_start;
   MPI_Reduce( &time_elapsed, &total_time_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );

   if( my_rank == 0 )
      printf( "Elapsed time = %e seconds\n", total_time_elapsed );
   
   MPI_Finalize();

   return 0;
}
