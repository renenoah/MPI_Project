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
#include <time.h>
#include <stdlib.h>

int create_random_array(int length_of_array, int array[], int rank_for_seed);
int calc_min_max_sum_from_array(int length_of_array,int input_array[],int result[]);
int combining_two_calculated_min_max_sum(int first[],int second[]);
int custom_pow_base_two(int faktor);

int main(int argc, char* argv[ ]) 
{ 
	/* some declaration of needed variables */
	int namelen;					// length of name
	int my_rank, size; 				// rank of the process, number of processes
	int tag = 0; 
	char *c, proc_name[MPI_MAX_PROCESSOR_NAME+1]; 	// hostname  
    int length;

	
	/* Start of MPI part*/

	MPI_Init(&argc, &argv);		 		// initializing of MPI-Interface
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 	// get your rank
	/*Code reacts to flag -h*/
	//Only the main Rank should answer -h
	if(argc < 2){
		printf("Keine richtigen Parameter. Bitte die Anzahl an Zufallsveriablen angeben.\n")	;
		return 0;
	}
    else{        
        int num = atoi(argv[1]);        
		length = num;
    }    


	// Hint: for more MPI Functions see mpich.org > documentation > manpages > v3.3 
	MPI_Comm_size(MPI_COMM_WORLD, &size); 


	memset (proc_name, 0, MPI_MAX_PROCESSOR_NAME+1 );
									// initialize string with NULL-characters
	MPI_Get_processor_name(proc_name, &namelen);
									// finding out own computer name

	if ( (c=strchr(proc_name,'.'))  !=  NULL) *c = '\0'; 	
									// separate the first part of hostname

    	/*Calculate the amount of steps needed.
	Probably more optimal Version possible, but prio is currently low on Optimizing this step and it caused a lot of headaches
	*/
	int lower = 1;
	int steps = 0;
	while(1){
		if (lower <= size-1 && (lower * 2)>size-1)break;
		else{
			steps++;
			lower <<=1;
		}
	}


	
	int random_numbers[length]; //array will be filled with random numbers
	int calculated_result[3]; // Array contains min, max, sum in this order
	create_random_array(length,random_numbers,my_rank);
	calc_min_max_sum_from_array(length,random_numbers,calculated_result);
	printf("Prozess: %d, min %d, max %d, sum %d\n",my_rank,calculated_result[0],calculated_result[1],calculated_result[2]);

	double time_at_start;
	if(my_rank == 0){
		time_at_start = MPI_Wtime();
	}


	for(int i = 0; i < steps+1;i++){
		
		/*Finding Partner*/
		int two_over_t = custom_pow_base_two(steps-i); 
		
		int pa = my_rank ^ two_over_t;
		//If the partner is an process, that is not existent. we wait till the next round
		if(pa > size-1){
			continue;
		}
		if(my_rank > pa){
			for(int i = 0; i < 3; i++){
				MPI_Send ( &calculated_result, 3, MPI_INT, pa, tag, MPI_COMM_WORLD);		
			}
			break;
		}
		else{
			//Recieving min max sum from last Prozess
			MPI_Status status; /* Status variable */	
			int ergebnis[3];
			for(int i = 0; i < 3; i++){
				MPI_Recv ( &ergebnis,3, MPI_INT, pa, tag, MPI_COMM_WORLD,&status );			
			}
			combining_two_calculated_min_max_sum(calculated_result,ergebnis);
		}

		printf("Wir sind aktuell in dem Prozess %d zwei hoch t %d  ist mein Partner %d miot der SIze %d \n",my_rank,two_over_t,pa,size);

	}
	if(my_rank == 0){
		printf("Ergebnis über alle Prozesse hinweg: min %d, max %d, sum %d\n",calculated_result[0],calculated_result[1],calculated_result[2]);  
		double time_at_end = MPI_Wtime();
		double seconds_between_begin_and_end = time_at_end - time_at_start;
		printf("Time passed between first and last Process %f. Thats means every Process needed approxamitly %f seconds.\n",seconds_between_begin_and_end,seconds_between_begin_and_end/size);   
	}



	MPI_Finalize();		// finalizing MPI interface

	/* End of MPI, no MPI Functions after finalizing the MPI interface */
 
	return 0;		// end of progam with exit code 0 
} 

int create_random_array(int length_of_array, int array[], int rank_for_seed){
        //Filling Array with random numbers
		srand(time(NULL) * rank_for_seed);  
        for(int i = 0; i < length_of_array; i++){
			array[i] = rand()%1000;
            // printf("%d   ",array[i]);
		}
}
int calc_min_max_sum_from_array(int length_of_array,int input_array[],int result[]){
	result[0] = input_array[0]; //min
    result[1] = input_array[0];	//max
    result[2] = 0; //sum
	for(int i = 0; i < length_of_array; i++){
		result[2] = result[2] + input_array[i];
		if(result[0] > input_array[i]) result[0] = input_array[i];
		if(result[1] < input_array[i]) result[1] = input_array[i];
	}
}
int combining_two_calculated_min_max_sum(int first[],int second[]){
	if(first[0] > second[0])first[0] = second[0];
	if(first[1] < second[1])first[1] = second[1];
	first[2] = first[2] + second[2];
}

int custom_pow_base_two(int faktor){
	if(faktor == 0)return 1;
	else{
		int answer = 1;
		for(int i = 0;i < faktor;i++){
			answer <<= 1;
		}
		return answer;
	}
}