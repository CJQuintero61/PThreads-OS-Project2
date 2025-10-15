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

// create a Dimensions datatype to get the rows and columns
typedef struct {
    int rows;
    int cols;
} Dimensions;

// prototypes
Dimensions getDimensions();
void fillMatrix(int** A, int* x, const Dimensions d);

int main() {
    // get the rows and cols
    const Dimensions dimensions = getDimensions();

    // allocate the rows for the A matrix
    int** A = (int*) malloc(dimensions.rows * sizeof(int));     

    // allocate the columns for each row in the A matrix
    for(int i = 0; i < dimensions.rows; i++) {
        A[i] = (int*) malloc(dimensions.cols * sizeof(int));            
    }

    // repeat for the Y matrix
    int** Y = (int*) malloc(dimensions.rows * sizeof(int));

    for(int i = 0; i < dimensions.rows; i++) {
        Y[i] = (int*) malloc(sizeof(int));            
    }

    // allocate the x vector
    // it only needs to be of size cols
    int* x = (int*) malloc(dimensions.cols * sizeof(int));

    // fill the A matrix and x vector with random integers
    fillMatrix(A, x, dimensions);









    // cleanup memory allocation and avoid dangling pointers
    free(A);
    A = NULL;
    free(Y);
    Y = NULL;
    free(x);
    x = NULL;

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
            A: int* - pointer to the A matrix. A 2D array with rows = d.rows and cols = d.cols
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



