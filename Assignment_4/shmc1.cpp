/*
Brian Powell 012362894
CECS 326 - Shui Lam
Assignment 4

File: shmc1.cpp
*/

//includes
#include "registration.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <memory.h>
#include <semaphore.h>


using namespace std;

//global variables
#define SNAME "/mutualex"
CLASS 	*class_ptr; //points to class data type
void 	*memptr; //points to memory location
char	*pname; //points to process name
int		shmid, ret; //shared memory id, return id


sem_t *semaphore = sem_open(SNAME, 0);

void rpterror(char *), srand(), perror(), sleep();
void sell_seats(); 

main(int argc, char* argv[])
{
	if (argc < 2) { //checks if child names are less than 2
		fprintf (stderr, "Usage:, %s shmid\n", argv[0]); //prints error to standard output stream
		exit(1); //exits 
	}

	pname = argv[0]; //assign pname to the first argument 
	sscanf (argv[1], "%d", &shmid); //scanf reads from character string from argv[1] to the memory location

	memptr = shmat (shmid, (void *)0, 0); //attached memptr to the shared memory segment identified by shmid
	
		
	if (memptr == (char *)-1 ) { //checks to see if shared memory location exists
		rpterror ("shmat failed"); //print error
		exit(2); //exit with error code 2
	}

	class_ptr = (struct CLASS *)memptr; //assign class_ptr of type CLASS to the memptr location

	sell_seats(); //call sell_seats function

	ret = shmdt(memptr); //detached piece from shared memory location and assign it to ret
	exit(0); //exit with code 0
}

void sell_seats() 
{
	int all_out = 0; //variable for all seats empty

	srand ( (unsigned) getpid() ); //get random process id
	while ( !all_out) {   /* loop to sell all seats */
		sem_wait(semaphore); //wait for semaphore to be released
		if (class_ptr->seats_left > 0) { 
			sleep ( (unsigned)rand()%5 + 1);
			class_ptr->seats_left--;
			sleep ( (unsigned)rand()%5 + 1);
			cout << pname << " SOLD SEAT -- " 
			     << class_ptr->seats_left << " left" << endl;
			sem_post(semaphore); //CS complete, release semaphore
		}else{
			sem_post(semaphore); //CS complete, release semaphore
			all_out++;
			cout << pname << " sees no seats left" << endl;
		}
		sleep ( (unsigned)rand()%10 + 1);
	}
}

void rpterror(char* string)
{
	char errline[50]; //error message of length char[50]

	sprintf (errline, "%s %s", string, pname);  //converts error message from a char to string array to print
	perror (errline); //prints error line
}
