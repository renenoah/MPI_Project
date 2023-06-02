/************************************************************************/
/* Program: main.c                                                 */ 
/* Author: Rene Noah Kouache    s1750463@stud.uni-farnkfurt.de                                 */
/* matriclenumber:   5782459                                                   */
/* Assignment : 2                                                       */	
/* Task: 2                                                        */
/* Parameters:                                                       */
/* Environment variables: no                                            */
/*                                                                      */
/* Description:   Each Process calculates on his intervall the  3/8 Simpson Rule                                                  */

/* The result will be send too all processes with the butterfly Algorithmus                                                                      */
/*                                                                   */	
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
#include <math.h>
#include "/home/lab/2023/s1750463/libs/help.h"

double first_math_function(double x);
double second_math_function(double x);

int main(int argc, char* argv[ ]){    

	int my_rank, size; 				// rank of the process, number of processes
	MPI_Status status;
    


    #define BUFFER_SIZE 100
    char buffer[BUFFER_SIZE];       
    int number_of_intervals;
    double integration_lower_limit, integration_upper_limit;
    int selected_function;
    int pos = 0;


	MPI_Init(&argc, &argv);		 		// initializing of MPI-Interface
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 	// get your rank    
	MPI_Comm_size(MPI_COMM_WORLD, &size); 
    if(my_rank == 0){        
        printf("Wilkommen\n");
        printf("Bitte die Anzahl an Intervallen angeben. Nur Ganzzahlen sind erlaubt und teilbar durch 3\n");
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

    }
    double (*math_func)(double);
    if(selected_function == 1){
        math_func = first_math_function;
    }
    else{
        math_func = second_math_function;
    }
    double h = (integration_upper_limit - integration_lower_limit) / (double)size;
    double local_a = integration_lower_limit + my_rank * h;
    double local_b = integration_lower_limit + (my_rank+1) * h;
    double local_h =  (local_b - local_a) / (double)number_of_intervals;
    
    //Ergebnis berechnen
    size_t length = (local_b-local_a)/local_h+1; 
    double x_werte[length];
    double k = 0 ;
    for(size_t i = 0 ; i < length ; i++ ){
        x_werte[i] = local_a + k * local_h; 
        k++;       
    }
    double result = 0, subresult_one = 0, subresult_two = 0;
    for(size_t i = 1; i < (length-1)/3 + 1; i++){
        subresult_one += math_func(x_werte[3*i-2]) + math_func(x_werte[3*i-1]);
    }
    for(size_t i = 1; i < ((length-1)/3 - 1) +1 ; i++){
        subresult_two += math_func(x_werte[3*i]);
    }
    // printf("Local h: %f, subresult_one : %f , subresulttwo: %f\n",local_h,subresult_one,subresult_two);
    result = ((double)3.0 / (double)3.8) * local_h * (math_func(x_werte[0]) + (3*subresult_one) + (2* subresult_two) + math_func(x_werte[length-1]));
    printf("Prozess %d: Errechnetes Lokales Ergebnis %f für die a: %f bis b:%f mit h:%f\n",my_rank,result,local_a,local_b,local_h);

    //Butterfly Algo
    int mask = 1<< ((int)log2(size)-1);
    double recieved_result = 0;
    int partner_rank = 0;
    MPI_Request recieve_req;
    MPI_Request send_req;
    while(mask > 0){
        partner_rank = my_rank ^ mask;        
        MPI_Isend(&result,1,MPI_DOUBLE,partner_rank,0,MPI_COMM_WORLD,&send_req);
        MPI_Irecv(&recieved_result,1,MPI_DOUBLE,partner_rank,0,MPI_COMM_WORLD, &recieve_req);
        MPI_Wait(&send_req,&status);
        MPI_Wait(&recieve_req,&status);
        result += recieved_result;
        mask >>=1;
    }
    printf("Prozess: %d Gesamtergebnis: %f\n",my_rank,result);


    MPI_Finalize();		// finalizing MPI interface
    
    return 0;
}
double first_math_function(double x){
    return sqrt(3*x+2);
}
double second_math_function(double x){
    return (log(7*x))/x;
}

