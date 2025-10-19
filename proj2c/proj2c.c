/*
	File: proj2cc.c
	Created: 10/17/2025 by Christian Quintero
	Last Updated: 10/18/2025 by Christian Quintero

	Description:
    This program gets an input string from the user then makes 2 threads 
    to prind the output where one thread is responsible for printing the words in the input 
    string that start with a consonant and the other printing the words that start with a
    vowel. 

    The challenge here is to print the words in the same order as they were inputted
    without using mutexes. This is done by using an atomic counter that both threads check
    to see if it's their turn to print. 
  
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
#include <stdatomic.h>

#define MAX_INPUT_SIZE 200

// prototypes
void* print_vowels();
void* print_consonants();

// globals
_Atomic int counter = 0;                // atomic counter to keep track of the current token being processed
int is_vowel[MAX_INPUT_SIZE];           // array to mark if the token is a vowel or consonant
char* tokens_array[MAX_INPUT_SIZE];     // array to hold the tokens
int token_count = 0;                    // total number of tokens



int main() {
    // variable declarations
    char input_string[MAX_INPUT_SIZE];                  // the initial input string
    char* saveptr;                                      // 
    char* token;                                        // to hold each token                
    pthread_t vow_thread, con_thread;                  // thread identifiers

    // prompt the user for input
    printf("Enter an input string: ");
    fgets(input_string, MAX_INPUT_SIZE, stdin);

    // tokenize the input string
    token = strtok_r(input_string, " \n", &saveptr);
    while(token != NULL) {

        // store the token in the tokens array (this initially starts at 0)
        tokens_array[token_count] = token;

        // check if the first character is a vowel or consonant
        char c = token[0];
        if (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U' ||
            c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            is_vowel[token_count] = 1; // mark as vowel
        } else {
            is_vowel[token_count] = 0; // mark as consonant
        }

        // increase the token count and get the next token
        token_count++;
        token = strtok_r(NULL, " \n", &saveptr);
    }

    // create the threads
    pthread_create(&vow_thread, NULL, print_vowels, NULL);
    pthread_create(&con_thread, NULL, print_consonants, NULL);

    // join the threads
    pthread_join(vow_thread, NULL);
    pthread_join(con_thread, NULL);
    
    return 0;
} // end main


void* print_vowels() {
    /*
        This function prints the words that start with a vowel
        and waits for its turn based on the atomic counter.
    */

    for(int i = 0; i < token_count; i++) {
        // skip if not a vowel
        if( !is_vowel[i] ) {
            continue; 
        }

        // wait for its turn
        // this is done by busy waiting until the counter matches the current index
        while(atomic_load(&counter) != i ) {
            // busy wait
        }

        // print the vowel word
        printf("Vow: %s\n", tokens_array[i]);

        // increment the counter
        atomic_fetch_add(&counter, 1);
    }
    return NULL;
} // end print_vowels


void* print_consonants() {
    /*
        This function prints the words that start with a consonant
        and waits for its turn based on the atomic counter.
    */

    for(int i = 0; i < token_count; i++) {
        // skip if not a consonant
        if( is_vowel[i] ) {
            continue; 
        }

        // wait for its turn
        // this is done by busy waiting until the counter matches the current index
        while(atomic_load(&counter) != i ) {
            // busy wait
        }

        // print the consonant word
        printf("Con: %s\n", tokens_array[i]);

        // increment the counter
        atomic_fetch_add(&counter, 1);
    }
    return NULL;
} // end print_consonants





