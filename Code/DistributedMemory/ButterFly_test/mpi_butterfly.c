#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

long long Read_TotalN()
{
   char word[ 10 ];

   FILE *fp;
   fp = fopen( "gs_data", "r" );
   fscanf( fp, "%s", word );

   long long total_n = atoll( word );
   
   return total_n;
}

void Read_Array( long long total_n, double* array )
{
   char word[ 10 ];

   FILE *fp;
   fp = fopen( "gs_data", "r" );
   fscanf( fp, "%s", word );

   // array = malloc( total_n * sizeof(double) ); // This doesn't work.
   for( long long i = 0; i < total_n; i++ )
   {
      fscanf( fp, "%s", word );
      array[i] = atof( word );
   }

   fclose( fp );
}

double Compute_Partial_Sum( int my_rank, int comm_sz,
			    long long partial_n, double* array, double* sub_array )
{
   double partial_sum = 0.0;
   
   if( my_rank == 0 )
   {
      // --------------------------------------
      // Send data-----------------------------

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
   }


   if( my_rank != 0 )
   {
      // --------------------------------------
      // Receive partial array ----------------

      MPI_Recv( &partial_n, 1,        MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
      MPI_Recv( sub_array, partial_n, MPI_DOUBLE,    0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE );

      // --------------------------------------
      // Compute local sum---------------------

      for( long long i = 0; i < partial_n; i++ )
      {
	 partial_sum += sub_array[i];
      }
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

   long long total_n = 0;
   double* array = NULL;
   if( my_rank == 0 )
   {
      total_n = Read_TotalN();
      printf( "total_n = %lld\n", total_n );

      array = malloc( total_n*sizeof(double) );
      Read_Array( total_n, array );
      
      long long i;
      for( i = 0; i < total_n; i++ )
      {
	 //printf( "%2lld |  %f\n", i, array[i] );
      }
   }

   long long partial_n = total_n / comm_sz;
   double* sub_array = malloc( partial_n * sizeof(double) );
   
   MPI_Barrier( MPI_COMM_WORLD );
   double time_start = MPI_Wtime();

   double partial_sum = Compute_Partial_Sum( my_rank, comm_sz,
					     partial_n, array, sub_array );

   double time_end_test = MPI_Wtime();
   double time_elapsed_test = time_end_test - time_start;
   printf( "rank %2d / %d |  time_elapsed_test = %f seconds\n", my_rank, comm_sz, time_elapsed_test );

   double sum = Compute_Sum_Butterfly( my_rank, comm_sz, partial_sum );
   printf( "rank %2d / %d |  partial_sum = %f  |  sum = %f\n", my_rank, comm_sz, partial_sum, sum );

/*
   double time_end = MPI_Wtime();
   double time_elapsed = time_end - time_start;

   double max_time_elapsed = 0.0;
   MPI_Reduce( &time_elapsed, &max_time_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );

   MPI_Barrier( MPI_COMM_WORLD );
   
   printf( "rank %2d / %d |  time_elapsed = %f seconds\n", my_rank, comm_sz, time_elapsed );
   if( my_rank == 0 )
      printf( "--- --- Max Elapsed time = %e seconds --- ---\n", max_time_elapsed );

   //printf( "Partial elapsed time = %e seconds\n", time_elapsed );

*/
   
   MPI_Finalize();

   free( array );
   free( sub_array );
   
   return 0;
}
