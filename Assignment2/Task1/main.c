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
int extract_wanted_numbers(int LENGTH,int array_local[],int array_recieved[],int extract_left_side);

int main(int argc, char* argv[ ]) 
{ 
	
    /* some declaration of needed variables */
	int my_rank, size; 				// rank of the process, number of processes
	int tag = 0; 
    const int LENGTH = 10; 
    int random_numbers[LENGTH];
    int random_numbers2[LENGTH];

	/* Start of MPI part*/

	MPI_Init(&argc, &argv);		 		// initializing of MPI-Interface
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 	// get your rank
	MPI_Comm_size(MPI_COMM_WORLD, &size); 

    
    create_random_array(LENGTH, random_numbers, my_rank);// Create an array of random numbers
    create_random_array(LENGTH, random_numbers2, 10);// Create an array of random numbers
    print_int_array(LENGTH,random_numbers);
    
    extract_wanted_numbers(LENGTH,random_numbers,random_numbers2,0);
    print_int_array(LENGTH,random_numbers);


    // qsort( random_numbers, LENGTH, sizeof(int),compare);

    // /*
    // First we will open the persitent channels to our left and right partner.
    // Special Case for the first an last Process
    // */
    // MPI_Request request_left_send = NULL;
    // MPI_Request request_left_recieve = NULL;
    // MPI_Request request_right_send = NULL;    
    // MPI_Request request_right_recieve = NULL;
    // MPI_Status status_left_send;
    // MPI_Status status_left_recieve;
    // MPI_Status status_right_send;    
    // MPI_Status status_right_recieve;
    // int recieved[LENGTH]; //Array that holds the values from Partner
    // int combined[LENGTH*2]; //Array that holds owned and recieved Values and target fpr quicksort    
    // if(my_rank == 0){
    //     // has no left Partner establish only right side       
    //     //right side
    //     MPI_Recv_init(&recieved,LENGTH,MPI_INT,my_rank+1,0,MPI_COMM_WORLD,request_right_recieve);
    //     MPI_Send_init(&random_numbers,LENGTH,MPI_INT,my_rank+1,0,MPI_COMM_WORLD,request_right_send);        
    // }
    // else if (my_rank == size-1){
    //     //has no right Partner establish only left side
    //     //left side
    //     MPI_Recv_init(&recieved,LENGTH,MPI_INT,my_rank-1,0,MPI_COMM_WORLD,request_left_recieve);
    //     MPI_Send_init(&random_numbers,LENGTH,MPI_INT,my_rank-1,0,MPI_COMM_WORLD,request_left_send);
    // }
    // else{
    //     //has left an right Partner
    //     //left side
    //     MPI_Recv_init(&recieved,LENGTH,MPI_INT,my_rank-1,0,MPI_COMM_WORLD,request_left_recieve);
    //     MPI_Send_init(&random_numbers,LENGTH,MPI_INT,my_rank-1,0,MPI_COMM_WORLD,request_left_send);
    //     //right side
    //     MPI_Recv_init(&recieved,LENGTH,MPI_INT,my_rank+1,0,MPI_COMM_WORLD,request_right_recieve);
    //     MPI_Send_init(&random_numbers,LENGTH,MPI_INT,my_rank+1,0,MPI_COMM_WORLD,request_right_send);
    // }
    // /*
    // we need n steps for worstcase array
    // */
    // for(int i = 0; i < size;i++){
    //     if(i%2==0){
    //         //We are in an even step
    //         //Even ranks and 0 should communicate with the right partner and get the left side of the 2*Lenght Array            
    //         //odd ranks should communicate with the left partner and hold the right side of the 2*Length Array
    //         if(my_rank%2==0){
    //             //We are in an even rank => communicate with right side
    //             if(request_right_send == NULL || request_right_recieve == NULL)continue;
    //             MPI_Start(&request_right_send);
    //             MPI_Start(&request_right_recieve);
    //             MPI_Wait(&request_right_recieve,&status_right_recieve);
    //             //As soon as we recieved our Array we can already start extracting only the numbers we need

    //             MPI_Wait(&request_right_send,&status_right_send);
    //             //MAGIC
    //         }
    //         else{
    //             //We are in an odd rank => communicate with left side
    //             if(request_left_send == NULL || request_left_recieve == NULL)continue;
    //             MPI_Start(&request_left_send);
    //             MPI_Start(&request_left_recieve);
    //             MPI_Wait(&request_right_recieve,&status_left_recieve);
    //             MPI_Wait(&request_right_send,&status_left_send);
    //             //MAGIC
    //         }
    //     }
    //     else{
    //         //We are in an odd step
    //         //Even ranks and 0 should communicate with the left partner and get the right side of the 2*Lenght Array            
    //         //odd ranks should communicate with the right partner and hold the left side of the 2*Length Array
    //         if(my_rank%2==0){
    //             //We are in an even rank
    //         }
    //         else{
    //             //We are in an odd rank
    //         }

    //     }
    // }

    // print_int_array(LENGTH,random_numbers);
    


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
int extract_wanted_numbers(int LENGTH,int array_local[],int array_recieved[],int extract_left_side){
    //Combine both arrays
    int combined[LENGTH+LENGTH];
    for(int i = 0; i < LENGTH;i++){
        combined[i] = array_local[i];
    }
    for(int i = LENGTH; i < LENGTH*2;i++){
        combined[i] = array_recieved[i-LENGTH];
    }    
    qsort( combined, LENGTH+LENGTH, sizeof(int),compare);
    print_int_array(LENGTH+LENGTH,combined);
    if(extract_left_side){
        printf("Bin hier \n");
        for(int i = 0; i < LENGTH;i++){
            array_local[i] = combined[i];
        }
    }//!!!!! Nochmal überprüfen
    else{        
        for(int i = LENGTH; i < LENGTH*2;i++){
            array_local[i-LENGTH] = combined[i];
        }
    }
    return 0;
}