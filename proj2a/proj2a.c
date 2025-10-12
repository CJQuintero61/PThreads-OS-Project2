/*
	File: proj2a.c
	Created: 10/12/2025 by Christian Quintero
	Last Updated: 10/12/2025 by Christian Quintero

	Description:
	This file uses Monte Carlo methods to approximate PI then uses a number of 
	user entered threads to discover at what point, the time to compute can 
	no longer be decreased. 
  
  To Run:
  1. must be in a unix based terminal or wsl
  2. `make` to compile
  3. `make run` to execute the program
  4. follow the prompts to enter the number of sample points and threads
  5. `make clean` to remove the compiled files

*/

// imports 
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

// preprocessor directives
#define MAX_THREADS 512

// prototypes
void* compute_pi( void* );

// global variables shared by all threads
int sample_points;                          // the amount of sample points to test
int total_hits;                             // the total number of hits                     
int total_misses;                           // the total number of misses
int hits[ MAX_THREADS ];                    // array to hold hits per thread 
int sample_points_per_thread;               // sample points divided among the threads
int num_threads;                            // number of threads to create                  


int main( int argc, char *argv[] ) {
  // local variables
  int ii;                                   // itertor
  int retval;                               // return value               
  pthread_t p_threads[MAX_THREADS];         // thread handles 
  pthread_attr_t attr;                      // thread attributes
  double computed_pi;                       // computed value of pi

  // itit variables
  retval = 0;
  total_hits = 0;

  // initialize the thread attributes
  pthread_attr_init( &attr );

  // set the scheduling policy to be system scope
  pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM );

  // get the user input
  // there is no validation here
  printf( "Enter number of sample points: " );
  scanf( "%d", &sample_points );

  printf( "Enter number of threads: " );
  scanf( "%d", &num_threads );
 
  // calculate the number of sample points per thread
  sample_points_per_thread = sample_points / num_threads;

  // create the threads
  for( ii=0; ii<num_threads; ii++ ) {
    // initialize the hits array
    hits[ii] = ii;

    // create the thread
    pthread_create( &p_threads[ ii ], &attr, compute_pi, (void *) &hits[ii] );
  }

  for( ii=0; ii<num_threads; ii++ ) {
    // wait for the thread to complete
    pthread_join( p_threads[ ii ], NULL );

    // accumulate the hits
    total_hits += hits[ ii ];
  }

  // calculate the computed value of pi
  computed_pi = 4.0 * (double) total_hits / ((double) (sample_points));

  // print the results
  printf( "Computed PI = %lf\n", computed_pi );

  // return to the calling environment
  return( retval );
} // end main


void *compute_pi( void *s ) {

  // local variables
  int seed;                       // seed for the random number generator
  int ii;                         // iterator
  int* hit_pointer;               // pointer to the hits for this thread
  int local_hits;                 // local hits for this thread
  double rand_no_x;               // random x coordinate
  double rand_no_y;               // random y coordinate

  // initialize the local variables
  hit_pointer = (int* ) s;
  seed = *hit_pointer;
  local_hits = 0;

  // generate the random numbers and determine if they are a hit or miss
  for( ii=0; ii < sample_points_per_thread; ii++ ) {

    rand_no_x = (double) (rand_r( &seed ))/(double)RAND_MAX;

    rand_no_y = (double) (rand_r( &seed ))/(double)RAND_MAX;

    if(((rand_no_x - 0.5) * (rand_no_x - 0.5) + (rand_no_y - 0.5) * (rand_no_y - 0.5)) < 0.25)
      local_hits++;

    seed *= ii;
  }

 
  // store the local hits in the global hits array
  *hit_pointer = local_hits;

  pthread_exit(0);
} // end compute_pi
