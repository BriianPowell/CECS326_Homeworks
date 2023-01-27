/*
Brian Powell 012362894
CECS 326 - Shui Lam
Assignment 2

File: sender.cpp
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
	int qid = msgget(ftok(".", 'u'), 0);

	//data structure for message to be sent
	struct buf{
		long mtype; //required for message queue to work properly
		char greeting[50]; //message to be sent
	};

	//declare message object
	buf msg;
	int size = sizeof(msg) - sizeof(long);
	char message[50];

	//get message to be sent from user input
	//copy char array to msg.greeting object to prepare it to be sent
	//use msgsnd() with message queue identifier to send message
	cout << "=================================" << endl;
	cout << "[Executing From sender.cpp]" << endl;
	cout << "PID: [" << getpid() << "]" << endl;
	cout << "Enter message to send: ";
	cin.getline(message, sizeof(message));
	cout << endl;


	strcpy(msg.greeting, message);

	//from man msgrcv: System V message queue operations
	//use to send messages to a System V message queue
	msg.mtype = 113; 
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

	cout << "[Message Sent]" << endl;
	cout << "[sender.cpp, Child Process: " << getpid() << ", will now exit]" << endl;
	cout << "=================================" << endl << endl;
	exit(0); //exit child process
}
