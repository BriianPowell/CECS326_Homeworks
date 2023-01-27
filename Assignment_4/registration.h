/*
Brian Powell 012362894
CECS 326 - Shui Lam
Assignment 4

The implementations of semamphore are commonly available on most distributions of UNIx and Linux operating systems. The system V 
implementation includes semget(), semctl(), and semop(), and a struct sembuf used in the semop() calls, which are defined in <sys/sem.h>.
The POSIX implementation includes sem_wait(), sem_post(), and sem_open(), sem_close() & sem_unlink() for named semaphore and sem_init() &
sem_destroy() for unnamed semaphore, which are definted in <semaphore.h>. You may use either one of these implementations. Details on 
the definition of these functions and their use may be found on Linux man pages.

File: registration.h
*/

struct CLASS {
	char class_number[6]; //number of class
	char date[7]; //date of class
	char title[50]; //title of class
	int seats_left; //number of seats left in class
};
