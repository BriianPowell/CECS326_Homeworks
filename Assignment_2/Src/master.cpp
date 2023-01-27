/*
Brian Powell 012362894
CECS 326 - Shui Lam
Assignment 2

For this assignment  you need to write three c++ programs named master.cpp, 
sender.cpp, and receiver.cpp, which should be compiled into executables master, 
sender, and receiver, respectively. Together they should do the following:

The mater process should first acquire a message queue from the operating system, 
followed by creating two child precesses, with one to execute sender and the other 
to execute receiver. Master should output the message queue ID, the process IDs of 
the sender and receiver processes it has created, then waits for both child processes
to return.
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

int main() {

	//from man msgget: get a System V message queue identifier
	//IPC_EXCL & IPC_CREAT allow the system to understand if a message queue with the identifier exists already, else create one
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	cout << "=================================" << endl;
	cout << "[master.cpp]" << endl;
	cout << "[Executing]" << endl;
	
	//first child process
	//waits for child to execute then exits
	pid_t cpid = fork(); 
	if (cpid == 0) //child process check and call /sender
	{
		cout << "[Sender, PID: " << cpid << "]" << endl;
		cout << "[Executing From master.cpp]" << endl << endl;
		execl("./sender","sender", (char *) NULL);
		exit(0);
	}

	//second child process
	//waits for child to execute then exits
	cpid = fork();
	if (cpid == 0) //child process check and call /receiver
	{
		cout << "[Receiver, PID: " << cpid << "]" << endl;
		cout << "[Executing From master.cpp]" << endl << endl;;
		execl("./receiver", "receiver", (char *) NULL);
		exit(0);
	}

	//wait for all child processes to return 
	while(wait(NULL) != -1);

	//from man msgctl: System V message control operations
	//performs the control operation specified by cmd on the System V message queue with identifier msqid
	//IPC_RMID will immediately remove the message queue and all waiting reader and writer processes
	msgctl(qid, IPC_RMID, NULL);

	//exit parents process and display process id
	cout << "=================================" << endl;
	cout << "[master.cpp]" << endl;
	cout << "PID: [" << getpid() << "]"  << endl;
	cout << "[Destroying Message Queue and Exiting]" << endl;
	cout << "=================================" << endl;
	exit(0);
}
