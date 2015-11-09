#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

double a = 0.0;
double b = 10.0;

int n;
int thread_count;

volatile int flag = 0;
volatile double sum = 0.0;

double F( double x )
{
   return x*x;
}

void* BusyWait( void* rank )
{
   double h = (b-a) / n;
   int my_rank = (int)rank;
   int    local_n = n / thread_count;
   double local_a = a + my_rank * local_n * h;
   double local_b = local_a + local_n * h;

   double my_sum = ( F(local_a) + F(local_b) ) / 2.0;
   int i;
   for( i = 1; i < local_n; i++ )
   {
      double x;
      x = local_a + i * h;
      my_sum += F(x);
   }
   my_sum *= h;


   while( flag != my_rank );
   printf( "Add from rank # %d  |  my_sum = %10.6f\n", my_rank, my_sum );

   sum += my_sum;
   flag++;

   return NULL;
}

int main( int argc, char* argv[] )
{
   int      thread;
   pthread_t* thread_handles;

   n =            strtol( argv[1], NULL, 10 );
   thread_count = strtol( argv[2], NULL, 10 );
   thread_handles = malloc( thread_count * sizeof(pthread_t*) );

   for( thread = 0; thread < thread_count; thread++ )
   {
      pthread_create( &thread_handles[thread], NULL, BusyWait, (void*)thread );
   }

   for( thread = 0; thread < thread_count; thread++ )
   {
      pthread_join( thread_handles[thread], NULL );
   }

   free( thread_handles );

   printf( "Sum = %10.6f\n", sum );
   
   return 0;
}
