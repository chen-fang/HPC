#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

long n;
long thread_count;

//long flag = 0;
//double sum = 0.0;

volatile long flag = 0;
volatile double sum = 0.0;

double Compute_PI_Serial( long n )
{
   double factor;
   double local_sum = 0.0;
   long long my_n = n/thread_count;
   long long i;
   for( i = 0; i < thread_count; i++ )
   {
      long long my_first_i = i * my_n;
      long long my_last_i = my_first_i + my_n;

      if( my_first_i % 2 == 0 )
	 factor = 1.0;
      else
	 factor = -1.0;

      double segment_sum = 0.0;
      long long j;
      for( j = my_first_i; j < my_last_i; j++, factor = -factor )
      {
	 segment_sum += factor/(2*j+1);
      }

      local_sum += segment_sum;
   }

   return local_sum;
}

void* Compute_PI( void* rank )
{
   long my_rank = (long)rank;
   double factor;
   long long my_n = n/thread_count;
   long long my_first_i = my_n * my_rank;
   long long my_last_i = my_first_i + my_n;

   if( my_first_i % 2 == 0 )
      factor = 1.0;
   else
      factor = -1.0;

   double my_sum = 0.0;
   long long i;
   for( i = my_first_i; i < my_last_i; i++, factor = -factor )
   {
      double tmp = factor/(2*i+1);
      my_sum += tmp;
   }

   while( flag != my_rank )
   sum += my_sum;
   flag++;

   return NULL;
}

int main( int argc, char* argv[] )
{
   long thread;
   pthread_t* thread_handles;

   n = strtol( argv[1], NULL, 10 );
   thread_count = strtol( argv[2], NULL, 10 );
   thread_handles = malloc( thread_count * sizeof(pthread_t*) );

   for( thread = 0; thread < thread_count; thread++ )
   {
      pthread_create( &thread_handles[thread], NULL, Compute_PI, (void*)thread );
   }

   for( thread = 0; thread < thread_count; thread++ )
   {
      pthread_join( thread_handles[thread], NULL );
   }

   free( thread_handles );

   double sum_serial = Compute_PI_Serial( n );
   
   printf( "PI_thread = %.20f\nPI_serial = %.20f | equal? %d\n", sum, sum_serial, sum==sum_serial );
   
   
   return 0;
}
