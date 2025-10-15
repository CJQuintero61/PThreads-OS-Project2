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

// create a Dimensions datatype to get the rows and columns
typedef struct {
    int rows;
    int cols;
} Dimensions;

// prototypes
Dimensions getDimensions();

int main() {
    // get the rows and cols
    Dimensions dimensions = getDimensions();

    printf("Dimensions:\n Rows: %d     Cols: %d", dimensions.rows, dimensions.cols);
}

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



