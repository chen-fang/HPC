#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main( int argc, char* argv[] )
{
   int threadN = strtol( argv[1], NULL, 10 );
   int iterN = strtol( argv[2], NULL, 10 );

   int i;
/*   
#  pragma omp parallel for num_threads( threadN ) \
   schedule( static, 1 )
*/
#  pragma omp parallel for num_threads( threadN )
   for( i = 0; i < iterN; i++ )
   {
      int my_rank = omp_get_thread_num();
      int thread_count = omp_get_num_threads();
      
      printf( "%d ||  my_rank = %d  |  thread_count = %d\n", i, my_rank, thread_count );
   }
   
   return 0;
}
