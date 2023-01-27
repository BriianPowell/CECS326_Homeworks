/*
Brian Powell 012362894
CECS 326 - Shui Lam
Assignment 4

File:shmp1.cpp
*/

//includes
#include "registration.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <memory.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>

using namespace std;

//global variables
CLASS myclass = { "1001", "120186", "Operating Systems", 15 }; //initialize class variables
#define NCHILD	3
#define SNAME "/mutualex"	//named semaphore variable

//initialize and open a named semaphore
//requires <fcntl.> and <semaphore.h>
//sname is name of semaphore, O_CREATE is the create flag, control bits and value set to 1
sem_t *mutex = sem_open(SNAME, O_CREAT, 0600, 1); 


//function definitions
int	shm_init( void * );
void	wait_and_wrap_up( int [], void *, int );
void	rpterror( char *, char * );

main(int argc, char *argv[])
{
	//creates child array of size NCHILD, 
	//initiates i, shmid, semid 
	int 	child[NCHILD], i, shmid, semid;
	void	*shm_ptr; //creates void pointer for shared memory
	char	ascshmid[10], ascsemid[10], pname[14]; //declares char arrays for the process name and shared memory id's

	strcpy (pname, argv[0]);//copies calling process id to pname
	shmid = shm_init(shm_ptr); //initilizes shared memory and passes the ptr to shmid
	sprintf (ascshmid, "%d", shmid); //formats the shared memory id to signed decimal integer and copies is to ascshmid

	//create child processes to consume the shared memory locations
	for (i = 0; i < NCHILD; i++) { 
		child[i] = fork(); //creates child processes
		switch (child[i]) { //either throws error or forks to shmc1 consumer file
		case -1: //fork error
			rpterror ("fork failure", pname);
			exit(1);
		case 0: //fork returns successful
			sprintf (pname, "shmc%d", i+1); //print child name
			execl("shmc1", pname, ascshmid, (char *)0); //execute shmc1 and pass the child name, and shared memory location, null pointer
			perror ("execl failed"); //if execl failes
			exit (2); //exit case
		}
	}
	wait_and_wrap_up (child, shm_ptr, shmid);
}

//function defintion to initialize a shared memory space
int shm_init(void *shm_ptr)
{
	//return address of shared memory
	int	shmid;

	//allocates a System V shared memory segment(key_t key, size_t size, int shmflg)
	//shmflg if IPC_CREATE | IPC_EXCL check for shared memory if no then create
	shmid = shmget(ftok(".",'u'), sizeof(CLASS), 0600 | IPC_CREAT);
	if (shmid == -1) { //checks for shared resource error
		perror ("shmget failed");
		exit(3); //exits with signal 3
	}
	//System V shared memory operations (int shmid, const, const void *shmaddr, int shmflg)
	//attaches the System V shared memory segment indetified by shmid to the address space 
	//of the calling process The attaching address is speficied by shamddr
	shm_ptr = shmat(shmid, (void * ) 0, 0); //smaddr set to null pointer, finds suitable unuded address to attach segment to
	if (shm_ptr == (void *) -1) { //checks for shmat return error
		perror ("shmat failed");
		exit(4); //exits with signal 4
	}
	//memcpy will copy memory area (void *dest, const void *src, size_t n);
	memcpy (shm_ptr, (void *) &myclass, sizeof(CLASS));  
	return (shmid); //returns shmid data address to point to
}

void wait_and_wrap_up(int child[], void *shm_ptr, int shmid)
{
	//variable definitions for wait 
	int wait_rtn, w, ch_active = NCHILD;

	while (ch_active > 0) { //wraps up all children processes
		wait_rtn = wait( (int *)0 ); //wait until last child returns
		for (w = 0; w < NCHILD; w++) //cycles through active childs
			if (child[w] == wait_rtn) { //checks to see if active child is waiting 
				ch_active--; //if child is waiting it will consume the child
				break;
			}
	}
	cout << "Parent removing shm" << endl;
	//detaches the shared memory segment located at the address specified by shmaddr
	//points to shared memory pointer created in shm_init
	shmdt (shm_ptr);
	//close named semaphore variable mutex
	sem_unlink(SNAME);
	sem_close(mutex); 
	//no error check occurs
	//performs the control operation specified by cmd on the System V shared memory
	//segment whose idetntifier is geven in shmid(int shmid, int cmd, stuct shmid_ds *buf)
	//IPC_RMID marks the segment to be destroyed
	shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0);
	exit (0);
}

//prints error line of fork failure and the process name that failed to fork
void rpterror(char *string, char *pname)
{
	char errline[50]; //error message of length char[50]

	sprintf (errline, "%s %s", string, pname); //converts the string from a char array to string array to print
	perror (errline); //prints error line
}
