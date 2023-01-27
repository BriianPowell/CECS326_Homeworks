/*
Brian Powell 012362894
CECS 326 - Shui Lam
Assignment 3

Cooperating processes need to communicate between them.  Another way Linux supports interprocess communication is shared memory. A System V
 shared memory segment can be created and controlled using system calls shmget, shmctl, shmat, shmdt, and a C library function memcpy.  
 Please consult the man pages of these system calls for details.  This assignment is designed to illustrate a critical problem with processes 
 executing concurrently that try to access shared data.
For this assignment you need to copy the following two C++ programs (named shmp1.cpp and shmc1.cpp with a header file registration.h) into 
 your Linux directory, compile them into shmp1, and shmc1, respectively.  Then run shmp1 and observe what happens.  Run shmp1 at least 5 
 times and observe and report the results.

The program must run successfully on a Linux machine.

File: registration.h
*/

struct CLASS {
	char class_number[6]; //number of class
	char date[7]; //date of class
	char title[50]; //title of class
	int seats_left; //number of seats left in class
};
