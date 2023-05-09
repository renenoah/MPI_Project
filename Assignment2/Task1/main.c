/************************************************************************/
/* Program: a.c                                                  */ 
/* Author: Rene Noah Kouache    s1750463@stud.uni-farnkfurt.de                                 */
/* matriclenumber:   5782459                                                   */
/* Assignment : 1                                                       */	
/* Task: 2a                                                              */
/* Parameters: array lenght of random numbers                                                       */
/* Environment variables: no                                            */
/*                                                                      */
/* Description:                                                         */
/*                                                                      */
/*     Each process calculates min, max and sum of an array thats filled with random numbers                     */
/*      The size of the array is determined by an console input Parameter
 Each Process will send the solution to the next Process in line. The Proces will be send sequentally to all Processes one by one. The last process will send it to
 the first again. The first Process will then take the tim and print the answers.                                                                */	
/*                           */
/*                                         */
/*                                                                      */
/************************************************************************/ 

#include "mpi.h" 	    // import of the MPI definitions
#include <stdio.h> 	    // import of the definitions of the C IO library
#include <string.h>	    // import of the definitions of the string operations
#include <unistd.h>	    // standard unix io library definitions and declarations
#include <errno.h>	    // system error numbers
#include <time.h>
#include <stdlib.h>

int create_random_array(int length_of_array, int array[], int rank_for_seed);
void print_int_array(int length, int array[]);
int compare( void const * left, void const * right );

int main(int argc, char* argv[ ]) 
{ 
	
    /* some declaration of needed variables */
	int my_rank, size; 				// rank of the process, number of processes
	int tag = 0; 
    const int LENGTH = 10; 
    int random_numbers[LENGTH];

	/* Start of MPI part*/

	MPI_Init(&argc, &argv);		 		// initializing of MPI-Interface
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 	// get your rank
	MPI_Comm_size(MPI_COMM_WORLD, &size); 

    
    create_random_array(LENGTH, random_numbers, my_rank);// Create an array of random numbers
    print_int_array(LENGTH,random_numbers);
    qsort( random_numbers, LENGTH, sizeof(int),compare);
    print_int_array(LENGTH,random_numbers);
    


	MPI_Finalize();		// finalizing MPI interface

 
	return 0;		// end of progam with exit code 0 
} 

/*Recieves an Array and fills it with random numbers*/
int create_random_array(int length_of_array, int array[], int rank_for_seed){
        //Filling Array with random numbers
		srand(time(NULL) * rank_for_seed);  
        for(int i = 0; i < length_of_array; i++){
			array[i] = rand()%1000;
            // printf("%d   ",array[i]);
		}
}
//Hilfsfunktion zum printen von arrays
void print_int_array(int length, int array[]){
    for(int i = 0;i < length;i++){
        printf("Pos %d: %d  ",i,array[i]);        
    }
    printf("\n");
}

int compare( void const * left, void const * right )
{
  int* left_int = (int*) left;
  int* right_int = (int*) right;
  return (*left_int) > (*right_int);
}