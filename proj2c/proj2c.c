/*
	File: proj2cc.c
	Created: 10/17/2025 by Christian Quintero
	Last Updated: 10/17/2025 by Christian Quintero

	Description:
    This program gets an input string from the user then makes 2 threads 
    to prind the output where one thread is responsible for printing the words in the input 
    string that start with a consonant and the other printing the words that start with a
    vowel. 
  
    To Run:
    1. must be in a unix based terminal or wsl
    2. `make` to compile
    3. `make run` to execute the program
    4. follow the input prompts. 
    5. `make clean` to remove the compiled files
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define MAX_INPUT_SIZE 200

// prototypes
void* print_vowels(void* args);
void* print_consonants(void* args);


int main() {
    // variable declarations
    char input_string[MAX_INPUT_SIZE];
    char vow_copy[MAX_INPUT_SIZE];
    char con_copy[MAX_INPUT_SIZE];
    pthread_t vow_thread, cons_thread;

    // prompt the user for input
    printf("Enter an input string: ");
    fgets(input_string, MAX_INPUT_SIZE, stdin);

    // copy the input string for each thread to use
    strcpy(vow_copy, input_string);
    strcpy(con_copy, input_string);


    // create the threads
    pthread_create(&vow_thread, NULL, print_vowels, (void*) vow_copy);
    pthread_create(&cons_thread, NULL, print_consonants, (void*) con_copy);

    // join the threads
    pthread_join(vow_thread, NULL);
    pthread_join(cons_thread, NULL);
    
    return 0;
} // end main


void* print_vowels(void* args) {
    /* 
        print_vowels
        This function prints the words that start with a vowel from the input string.

        params:
            args - the input string passed as a void pointer
    */


    char* str = (char*) args;                        // cast the void pointer to a char pointer
    char* saveptr = NULL;                            // save the location for strtok_r
    char* token = strtok_r(str, " \n", &saveptr);    // get the first token

    // print words that start with a vowel
    while(token != NULL) {

        if(token[0] == 'A' || token[0] == 'E' || token[0] == 'I' || token[0] == 'O' || token[0] == 'U' ||
           token[0] == 'a' || token[0] == 'e' || token[0] == 'i' || token[0] == 'o' || token[0] == 'u') {
            printf("Vow: %s\n", token);
        }
        // get the next token
        token = strtok_r(NULL, " \n", &saveptr);
    }

    return NULL;
} // end print_vowels


void* print_consonants(void* args) {
    /* 
        print_consonants
        This function prints the words that start with a consonant from the input string.

        params:
            args - the input string passed as a void pointer
    */
    char* str = (char*) args;
    char* saveptr = NULL;
    char* token = strtok_r(str, " \n", &saveptr);

    // print words that start with a consonant
    while(token != NULL) {

        // to check for a consonant, we check if it is not a vowel
        if( !(token[0] == 'A' || token[0] == 'E' || token[0] == 'I' || token[0] == 'O' || token[0] == 'U' ||
           token[0] == 'a' || token[0] == 'e' || token[0] == 'i' || token[0] == 'o' || token[0] == 'u')) {
            printf("Con: %s\n", token);
        }
        // get the next token
        token = strtok_r(NULL, " \n", &saveptr);
    }

    return NULL;
} // end print_consonants






