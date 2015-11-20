#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void Atomic( long n )
{
   int my_rank = omp_get_thread_num();
   int thread_count = omp_get_num_threads();

   int i;
   double my_sum = 0.0;
   for( i = 0; i < n; i++ )
   {
#     pragma omp atomic
      my_sum += 1.0;
   }
   printf( "%2d/%2d |  my_sum = %f\n", my_rank, thread_count, my_sum );
}

void Critical( long n )
{
   int my_rank = omp_get_thread_num();
   int thread_count = omp_get_num_threads();

   int i;
   double my_sum = 0.0;
   for( i = 0; i < n; i++ )
   {
#     pragma omp critical
      my_sum += 1.0;
   }
   printf( "%2d/%2d |  my_sum = %f\n", my_rank, thread_count, my_sum );
}

int main( int argc, char* argv[] )
{
   int thread_count = strtol( argv[1], NULL, 10 );
   long n = strtol( argv[2], NULL, 10 );

   double start, elapsed;
   
   start = omp_get_wtime();
#  pragma omp parallel num_threads(thread_count)
   Atomic( n );
   elapsed = omp_get_wtime() - start;
   printf( "elapsed (atomic) = %e\n", elapsed );

#  pragma omp parallel num_threads(thread_count)
   Critical( n );
   elapsed = omp_get_wtime() - start;
   printf( "elapsed (critical) = %e\n", elapsed );

   return 0;
}
