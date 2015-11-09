#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "timer.h"

double a = 0.0;
double b = 10.0;

long n;
long thread_count;

double sum = 0.0;
sem_t sem_sum;
sem_t sem_timer;

sem_t sem_count;
sem_t sem_barrier;
long counter = 0;
double max_elapsed = 0.0;

double F( double );
void* Trapezoid( void* );

int main( int argc, char* argv[] )
{
   n =            strtol( argv[1], NULL, 10 );
   thread_count = strtol( argv[2], NULL, 10 );

   pthread_t* thread_handles;
   thread_handles = malloc( thread_count * sizeof(pthread_t) );

   sem_init( &sem_sum, 0, 1 );
   sem_init( &sem_timer, 0, 1 );
   sem_init( &sem_count, 0, 1 );
   sem_init( &sem_barrier, 0, 0 );


   
   long thread;
   for( thread = 0; thread < thread_count; thread++ )
   {
      pthread_create( &thread_handles[thread], NULL, Trapezoid, (void*)thread );
   }

   for( thread = 0; thread < thread_count; thread++ )
   {
      pthread_join( thread_handles[thread], NULL );
   }

   free( thread_handles );
   
   sem_destroy( &sem_sum );
   sem_destroy( &sem_timer );
   sem_destroy( &sem_count );
   sem_destroy( &sem_barrier );

   printf( "Sum = %10.6f\n", sum );
   printf( "Max time = %e\n", max_elapsed );
   
   return 0;
}

double F( double x )
{
   return x*x;
}

void* Trapezoid( void* rank )
{

   double start = 0.0;
   double end = 0.0;
   
   sem_wait( &sem_count );
   if( counter != thread_count - 1 )
   {
      counter++;
      sem_post( &sem_count );
      sem_wait( &sem_barrier );
   }
   else
   {
      counter = 0;
      sem_post( &sem_count );
      long i;      
      for( i = 0; i < thread_count-1; i++ )
	 sem_post( &sem_barrier );
   }

   GET_TIME( start );

   
   double h = (b-a) / n;
   long my_rank = (long)rank;
   long   local_n = n / thread_count;
   double local_a = a + my_rank * local_n * h;
   double local_b = local_a + local_n * h;

   double my_sum = ( F(local_a) + F(local_b) ) / 2.0;
   long i;
   for( i = 1; i < local_n; i++ )
   {
      double x;
      x = local_a + i * h;
      my_sum += F(x);
   }
   my_sum *= h;


   GET_TIME( end );
   double elapsed = end - start;

   sem_wait( &sem_timer );
   printf( "%2ld |  Elapsed time = %e\n", my_rank, elapsed );
   if( elapsed > max_elapsed )
      max_elapsed = elapsed;
   sem_post( &sem_timer );

   sem_wait( &sem_sum );
   printf( "Add from rank # %ld  |  my_sum = %10.6f\n", my_rank, my_sum );
   sum += my_sum;
   
   sem_post( &sem_sum );

   return NULL;
}
