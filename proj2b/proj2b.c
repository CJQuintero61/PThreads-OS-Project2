/*
	File: proj2b.c
	Created: 10/14/2025 by Christian Quintero
	Last Updated: 10/14/2025 by Christian Quintero

	Description:
	This file uses threads to solve a system of linear equations using matrix multiplication.
    The process here will be to get the dimensions m * n (rows * cols) of matrix A,
    then use n to create the x vector of size n, then use Y = AX to solve for Y.
  
  To Run:
  1. must be in a unix based terminal or wsl
  2. `make` to compile
  3. `make run` to execute the program
  4. follow the input prompts
  5. `make clean` to remove the compiled files
*/

// imports 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// create a Dimensions datatype to get the rows and columns
typedef struct {
    int rows;
    int cols;
} Dimensions;

// create the args that will be passed to each thread
typedef struct {
    int** A;         // pointer to the A matrix
    int* x;          // pointer to the x vector
    int* Y;          // pointer to the Y vector
    int row;         // the row index this thread will calculate
    int cols;        // the number of columns in the A matrix and size of x vector
} Thread_args;

// prototypes
Dimensions getDimensions();
void fillMatrix(int** A, int* x, const Dimensions d);
void cleanup(int** A, int* x, int* Y, pthread_t* threads, Thread_args* args, const Dimensions d);
void printMatricies(int** A, int* x, int* Y, const Dimensions d);
void* calculateValue(void* args);

int main() {
    // get the rows and cols
    const Dimensions dimensions = getDimensions();

    // allocate an array of row pointers for the A matrix
    int** A = malloc((size_t) dimensions.rows * sizeof(int *));   
    
    // if the allocation for A fails, print an error, cleanup, and exit
    if (!A) { 
        perror("Failed to allocate A matrix rows.");
        cleanup(A, NULL, NULL, NULL, NULL, dimensions);
        exit(1); 
    }

    // allocate the columns for each row in the A matrix
    for(int i = 0; i < dimensions.rows; i++) {
        A[i] = malloc((size_t) dimensions.cols * sizeof(int));    
        
        // validate the allocation for each row
        if(!A[i]) {
            perror("Failed to allocate A matrix columns.");
            cleanup(A, NULL, NULL, NULL, NULL, dimensions);
            exit(1);
        }
    }

    // the x vector only needs to be of size cols (aka n)
    int* x = malloc((size_t) dimensions.cols * sizeof(int));

    // if the allocation for x fails, print an error, cleanup, and exit
    if (!x) { 
        perror("Failed to allocate x vector.");
        cleanup(A, x, NULL, NULL, NULL, dimensions);
        exit(1); 
    }

    // the result vector Y only needs to be of size rows (aka m)
    int* Y = malloc((size_t) dimensions.rows * sizeof(int));

    // if the allocation for Y fails, print an error, cleanup, and exit
    if (!Y) { 
        perror("Failed to allocate Y vector.");
        cleanup(A, x, Y, NULL, NULL, dimensions);
        exit(1); 
    }

    // fill the A matrix and x vector with random integers
    fillMatrix(A, x, dimensions);

    // allocate an array of threads to handle each row of the A matrix
    pthread_t* threads = malloc((size_t) dimensions.rows * sizeof(pthread_t));
    if (!threads) { 
        perror("Failed to allocate threads.");
        cleanup(A, x, Y, threads, NULL, dimensions);
        exit(1); 
    }

    // allocate an array of Thread_args to pass to each thread
    Thread_args* thread_args = malloc((size_t) dimensions.rows * sizeof(Thread_args));
    if (!thread_args) {
        perror("Failed to allocate thread args");
        cleanup(A, x, Y, threads, NULL, dimensions);
        exit(1);
    }
    // create the threads to handle each row of the A matrix
    for(int i = 0; i < dimensions.rows; i++) {

        // set the args for this thread
        // each thread's args will have the same variables
        // except for the row index which will be different
        thread_args[i].A = A;
        thread_args[i].x = x;
        thread_args[i].Y = Y;
        thread_args[i].row = i; 
        thread_args[i].cols = dimensions.cols; 

        // create the thread and pass the row index as the argument
        // for example) thread[0] calculates Y[0] by using the entire A[0] row and the x vector
        pthread_create(&threads[i], NULL, calculateValue, (void*) &thread_args[i]); 
    }

    // wait for each thread to complete
    for(int i = 0; i < dimensions.rows; i++) {
        pthread_join(threads[i], NULL);
    }

    // print the matricies for debugging purposes
    printMatricies(A, x, Y, dimensions);

    // cleanup memory allocation and avoid dangling pointers
    cleanup(A, x, Y, threads, thread_args, dimensions);
    return 0;
} // end main

Dimensions getDimensions() {
    /*  getDimensions()
        This function asks the user to enter the number of rows and 
        columns to make the A and Y matricies.

        Returns:
            d: Dimensions - a dimensions object with rows and columns set
    
    */
    Dimensions d;

    printf("Enter the number of rows for the matrix: ");
    scanf("%d", &d.rows);

    printf("Enter the number of columns for the matrix: ");
    scanf("%d", &d.cols);

    return d;
} // end getDimensions

void fillMatrix(int** A, int* x, const Dimensions d) {
    /*  fillMatrix()
        This function fills the A matrix and x vector with random integers
        from 0 to 99.

        Parameters:
            A: int** - pointer to the A matrix. A 2D array with rows = d.rows and cols = d.cols
            x: int* - pointer to the x vector. A 1D array with size = d.cols
            d: Dimensions - dimensions object with rows and columns set
    */

    // seed the random number generator
    srand(time(NULL));

    // fill the A matrix
    for(int i = 0; i < d.rows; i++) {
        for(int j = 0; j < d.cols; j++) {
            A[i][j] = rand() % 100; // random integer from 0 to 99
        }
    }

    // fill the x vector
    for(int i = 0; i < d.cols; i++) {
        x[i] = rand() % 100; // random integer from 0 to 99
    }

} // end fillMatrix

void cleanup(int** A, int* x, int* Y, pthread_t* threads, Thread_args* args, const Dimensions d) {
    /*  cleanup()
        This function frees the memory allocated for the arrays and 
        avoids dangling pointers.

        Parameters:
            A: int*** - reference to pointer to the A matrix. A 2D array with rows = d.rows and cols = d.cols
            x: int** - reference to pointer to the x vector. A 1D array with size = d.cols
            Y: int** - reference to pointer to the Y vector. A 1D array with size = d.rows
            threads: pthread_t* - pointer to the array of threads
            d: Dimensions - dimensions object with rows and columns set
    */

    // free the x vector if it was able to be allocated
    if (x) { 
        free(x);
        x = NULL;
    }


    // free the Y vector if was able to be allocated
    if (Y) { 
        free(Y);  
        Y = NULL;
    }
    
    // if A was allocated, free each row and then free A itself
    if(A) {
        // free each row
        for(int i = 0; i < d.rows; i++) {
            // free each row if it is not NULL
            if (A[i]) { 
                free(A[i]);
                A[i] = NULL;
            }
        }
        // free the array of row pointers
        free(A);
        A = NULL;
    }

    // free the threads array if it was able to be allocated
    if (threads) { 
        free(threads);  
        threads = NULL;
    }

    // free the thread args array if it was able to be allocated
    if (args) { 
        free(args);  
        args = NULL;
    }

} // end cleanup

void* calculateValue(void* args) {
    // cast the args back to the correct type
    Thread_args* thread_args = (Thread_args*) args;

    // initialize the sum, this is the result of the dot product
    int sum = 0;

    for(int j = 0; j < thread_args->cols; j++) {
        // for this specific row, multiply each element by the corresponding element in x
        // then add that to the sum
        sum = sum +  (thread_args->A[thread_args->row][j] * thread_args->x[j] );
    }

    // store the result in the Y vector at the index of this row
    thread_args->Y[thread_args->row] = sum;

    return NULL;
} // end calculateValue

void printMatricies(int** A, int* x, int* Y, const Dimensions d) {
    /*  printMatricies()
        This function prints the A matrix, x vector, and Y vector for debugging purposes.

        Parameters:
            A: int** - pointer to the A matrix. A 2D array with rows = d.rows and cols = d.cols
            x: int* - pointer to the x vector. A 1D array with size = d.cols
            Y: int* - pointer to the Y vector. A 1D array with size = d.rows
            d: Dimensions - dimensions object with rows and columns set
    */

    // print the A matrix
    printf("Matrix A:\n");
    for(int i = 0; i < d.rows; i++) {
        for(int j = 0; j < d.cols; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // print the x vector
    printf("Vector x:\n");
    for(int i = 0; i < d.cols; i++) {
        printf("%d\n", x[i]);
    }
    printf("\n");

    // print the Y vector
    printf("Vector Y:\n");
    for(int i = 0; i < d.rows; i++) {
        printf("%d\n", Y[i]);
    }

} // end printMatricies



