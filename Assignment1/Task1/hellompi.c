/************************************************************************/
/* Program: hellomp.c                                                   */ 
/* Author: Your Name <email address>                                    */
/* matriclenumber:                                                      */
/* Assignment : 1                                                       */	
/* Task: 1                                                              */
/* Parameters: no                                                       */
/* Environment variables: no                                            */
/*                                                                      */
/* Description:                                                         */
/*                                                                      */
/* hellompi is a simple example program for MPI                         */
/*                                                                      */
/* For each CPU, where the program is running,                          */
/* the hostname will be printed.                                        */
/*                                                                      */
/************************************************************************/ 

#include "mpi.h" 	    // import of the MPI definitions
#include <stdio.h> 	    // import of the definitions of the C IO library
#include <string.h>	    // import of the definitions of the string operations
#include <unistd.h>	    // standard unix io library definitions and declarations
#include <errno.h>	    // system error numbers


int main(int argc, char* argv[ ]) 
{ 

	/* 
		Hint for -h functionalities:
		argc = number of arguments, value is at least 1 = name of programm
		argv[ ] = arguments, at least one argument in argv[0] = name of programm

		if argv[2] contains "-h" process with rank 0 should print a help message
		explaining HOW to execute the programm and a short explanation of the
		needed command line arguments (an example may help as well)
		and ALL processes should then terminate
		otherwise the algorithm should be executed with the given arguments
		
		Error handling is optional. You may implement some kind of error
		handling or default values if you like but this is NOT mandatory.

		EACH solution should implement the -h functionality!
	*/


	/* some declaration of needed variables */
	int namelen;					// length of name
	int my_rank, size; 				// rank of the process, number of processes
	int tag = 0; 
	char *c, proc_name[MPI_MAX_PROCESSOR_NAME+1]; 	// hostname  

	
	/* Start of MPI part*/

	MPI_Init(&argc, &argv);		 		// initializing of MPI-Interface
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 	// get your rank
	/*Code reacts to flag -h*/
	//Only the main Rank should answer -h
	if(argc > 1 && my_rank == 0){
		if(strcmp(argv[1],"-h") == 0){
			char* help_text = "This Text ist meant to explain the function of the Programm. The Programm prints the Rank Number of all started Processes and the specific Computer where the Process is on.\n";
			printf("%s",help_text);
		}
		else{
			printf("You tried to execute the programming with Input Parameters. The only available Parameter is -h.");
		}
		
		return 0;
	}
	else if(argc > 1){
		return 0;
	}
	// Hint: for more MPI Functions see mpich.org > documentation > manpages > v3.3 
	MPI_Comm_size(MPI_COMM_WORLD, &size); 

	memset (proc_name, 0, MPI_MAX_PROCESSOR_NAME+1 );
									// initialize string with NULL-characters
	MPI_Get_processor_name(proc_name, &namelen);
									// finding out own computer name

	if ( (c=strchr(proc_name,'.'))  !=  NULL) *c = '\0'; 	
									// separate the first part of hostname

	if (my_rank == 0){
		printf("%03d: process running on %s!\n", my_rank, proc_name );
		/*
		Recieving all Ranks and hostname
		*/
		MPI_Status status; /* Status variable */	
		char sub_proc_name[10+MPI_MAX_PROCESSOR_NAME+1];	// Prozessornummer + Prozessname
		for(int rank = 1; rank < size; rank++){
			MPI_Recv ( &sub_proc_name, MPI_MAX_PROCESSOR_NAME+1, MPI_CHAR, rank, tag, MPI_COMM_WORLD,&status );
			printf("%s",sub_proc_name );
		}
	}
	else{
		/*
		All Subprocesses send String with rank and proc_name to rank 0
		*/
		char send_temp[9 + MPI_MAX_PROCESSOR_NAME+1];
		sprintf(send_temp, "Rank:%d %s!\n", my_rank, proc_name);
		MPI_Send ( &send_temp, MPI_MAX_PROCESSOR_NAME+1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
	}
	
									// print rank and hostname to display

	MPI_Finalize();		// finalizing MPI interface

	/* End of MPI, no MPI Functions after finalizing the MPI interface */
 
	return 0;		// end of progam with exit code 0 
} 

