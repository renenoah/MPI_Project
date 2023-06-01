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
#include "/home/lab/2023/s1750463/libs/help.h"

double first_math_function(double upper_bound,double);

int main(int argc, char* argv[ ]){
    printf("Hello World\n");
    int test[3];
    int length = 5;
    print_array(test,length);
    //Rank 0 needs to communicate with the user an recieving all data
        /* some declaration of needed variables */
	int my_rank, size; 				// rank of the process, number of processes
	int tag = 0; 
    const int LENGTH = 10; 


    #define BUFFER_SIZE 100
    char buffer[BUFFER_SIZE];       
    int number_of_intervals;
    double integration_lower_limit, integration_upper_limit;
    int selected_function;
    int pos = 0;

    // int random_numbers2[LENGTH];

	/* Start of MPI part*/

	MPI_Init(&argc, &argv);		 		// initializing of MPI-Interface
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 	// get your rank
	MPI_Comm_size(MPI_COMM_WORLD, &size); 
    if(my_rank == 0){        
        printf("Wilkommen\n");
        printf("Bitte die Anzahl an Intervallen angeben. Nur Ganzzahlen sind erlaubt.\n");
        scanf("%d",&number_of_intervals);
        printf("Bitte geben Sie die untere Integrationsgrenze an\n");
        scanf("%lf",&integration_lower_limit);
        printf("Bitte geben Sie die obere Integrationsgrenze an\n");
        scanf("%lf",&integration_upper_limit);
        printf("Welche Funktion soll berechnet werden.\n 1.) sqrt(3x+2)\n 2.) (ln(7x)/x)\nBitte 1 oder 2 angeben\n");
        scanf("%d",&selected_function); 
        MPI_Pack(&integration_lower_limit,1,MPI_DOUBLE,buffer,BUFFER_SIZE,&pos,MPI_COMM_WORLD);
        MPI_Pack(&integration_upper_limit,1,MPI_DOUBLE,buffer,BUFFER_SIZE,&pos,MPI_COMM_WORLD);
        MPI_Pack(&number_of_intervals,1,MPI_INT,buffer,BUFFER_SIZE,&pos,MPI_COMM_WORLD);
        MPI_Pack(&selected_function,1,MPI_INT,buffer,BUFFER_SIZE,&pos,MPI_COMM_WORLD);        
        MPI_Bcast(buffer,BUFFER_SIZE,MPI_PACKED,0,MPI_COMM_WORLD);
        

    }
    else{
        MPI_Bcast(buffer,BUFFER_SIZE,MPI_PACKED,0,MPI_COMM_WORLD);        
        MPI_Unpack(buffer,BUFFER_SIZE,&pos,&integration_lower_limit,1,MPI_DOUBLE,MPI_COMM_WORLD);
        MPI_Unpack(buffer,BUFFER_SIZE,&pos,&integration_upper_limit,1,MPI_DOUBLE,MPI_COMM_WORLD);
        MPI_Unpack(buffer,BUFFER_SIZE,&pos,&number_of_intervals,1,MPI_INT,MPI_COMM_WORLD);
        MPI_Unpack(buffer,BUFFER_SIZE,&pos,&selected_function,1,MPI_INT,MPI_COMM_WORLD);
        printf("Prozess %d:  %d, %f, %f, %d\n",my_rank,number_of_intervals,integration_lower_limit,integration_upper_limit,selected_function);

    }
    

    MPI_Finalize();		// finalizing MPI interface
    
    return 0;
}

