/*
Brian Powell 012362894
CECS 326 - Shui Lam
Assignment 2

File: receiver.cpp
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[])
{
	//get message queue ID of program
	int qid = msgget(ftok(".",'u'), 0);
	
	//data structure for message to be sent
	struct buf{
		long mtype; //required for message queue to work properly
		char greeting[50]; //message to be sent
	};

	//declare message object
	buf msg;
	int size = sizeof(msg) - sizeof(long);

	//from man msgrcv: System V message queue operations
	//use to receive message from a System V message queue
	msgrcv(qid, (struct msgbuf*)&msg, size, 113, 0);
	
	cout << "=================================" << endl;
	cout << "[Executing From receiver.cpp]" << endl;
	cout << "PID: [" << getpid() << "]" << endl;
	cout << "[Message Received]" << endl;
	cout << "Displaying Message: " << msg.greeting << endl; 

	cout << "[receiver.cpp, Child Process: " << getpid() << ", will now exit]" << endl;
	cout << "=================================" << endl << endl;
	exit(0); //exit child process
}
