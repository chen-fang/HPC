#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "timer.h"

void* F( void* p )
{
   //long my_rank = (long)p;
   //printf( "my pthread rank = %ld\n", my_rank );
   return NULL;
}

long thread_count;
int main( int argc, char* argv[] )
{
   thread_count = strtol( argv[1], NULL, 10 );
   
   pthread_t* thread_handles = malloc( thread_count * sizeof(pthread_t) );

   double start, end;

   long thread;
   GET_TIME( start );
   for( thread = 0; thread < thread_count; thread++ )
   {
      pthread_create( &thread_handles[thread], NULL, F, (void*)thread );
   }

   for( thread = 0; thread < thread_count; thread++ )
   {
      pthread_join( thread_handles[thread], NULL );
   }
   GET_TIME( end );
   

   free( thread_handles );
   double elapsed = end - start;
   printf( "Elapsed time = %e\n", elapsed );
   
   return 0;
}
