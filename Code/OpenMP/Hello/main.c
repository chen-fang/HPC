#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void Hello()
{
   int my_rank = omp_get_thread_num();
   int threadN = omp_get_num_threads();

   printf( "%d/%d\n", my_rank, threadN );
}

int main( int argc, char* argv[] )
{
   int threadN = strtol( argv[1], NULL, 10 );

#pragma omp parallel num_threads(threadN)
   Hello();
   
   return 0;
}
