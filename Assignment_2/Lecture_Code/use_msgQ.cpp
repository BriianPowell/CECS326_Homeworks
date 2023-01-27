#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
using namespace std;

//data structure for message to be sent
struct buf {
    long mtype; //required for message queue to work properly 
    char greeting[50]; //message to be sent
};

void child_proc_one(int qid) {
    //declare message object
    buf msg; 
    //creates size of message object
    int size = sizeof(msg) - sizeof(long); 

    //from man msgrcv: System V message queue operations
	//use to receive message from a System V message queue
    //receive message with type 113
    msgrcv(qid, (struct msgbuf*) &msg, size, 113, 0); 

    //display PID of process receiving message and display message
    cout << getpid() << ": gets message" << endl;
    cout << "message :" << msg.greeting << endl; 

    //concat adios to message
    //output PID of process sending reply
    //send message with type 114 to IPC
    strcat(msg.greeting, " and Adios."); 
    cout << getpid() << ": sends reply" << endl; 
    msg.mtype = 114; 
    //from man msgrcv: System V message queue operations
	//use to send messages to a System V message queue
    msgsnd(qid, (struct msgbuf *)&msg, size, 0); 
    cout << getpid() << ": now exits" << endl;
}

void child_proc_two(int qid) {
    //declare message object
    buf msg; 
    //creates size of message object
    int size = sizeof(msg) - sizeof(long); 

    //sends a message with type 12
    //message will never be read
    //from man msgrcv: System V message queue operations
	//use to send messages to a System V message queue
    msg.mtype = 12;
    strcpy(msg.greeting, "Fake message");
    msgsnd(qid, (struct msgbuf*)&msg, size, 0); 

    //prepares message to send
    //concats "Hello There" to message
    strcpy(msg.greeting, "Hello There");

    //displays process sending message to IPC
    //sets message type to 133
    //uses msgsnd() to send message to IPc
    cout << getpid() << ": sends greeting" << endl;
    msg.mtype = 113; 
    msgsnd(qid, (struct msgbuf*)&msg, size, 0); 

    //from man msgrcv: System V message queue operations
	//use to receive message from a System V message queue
    //receives message of type 1144 and displays it
    msgrcv(qid, (struct msgbuf*)&msg, size, 114, 0);
    cout << getpid() << ": gets reply" << endl; 
    cout << "reply: " << msg.greeting << endl; 
    cout << getpid() << ": now exits" << endl;  
}

int main() {

   	//from man msgget: get a System V message queue identifier
	//IPC_EXCL & IPC_CREAT allow the system to understand if a message queue with the identifier exists already, else create one
    int qid = msgget(IPC_PRIVATE, IPC_EXCL|IPC_CREAT|0600); //create queue

    //spawning first child
    //waits for child to execute then exits
    pid_t cpid = fork(); //child process check to call child_proc_one
        if(cpid == 0) {
        child_proc_one(qid); 
        exit(0); 
    }

    //second child process
	//waits for child to execute then exits
    cpid = fork();
    if(cpid == 0) {  //child process check to call child_proc_two
        child_proc_two(qid);
        exit(0); 
    }
    
    //waiting for all child processes to return
    while(wait(NULL) != -1); 

	//from man msgctl: System V message control operations
	//performs the control operation specified by cmd on the System V message queue with identifier msqid
	//IPC_RMID will immediately remove the message queue and all waiting reader and writer processes
	
    msgctl(qid, IPC_RMID, NULL);

    //exit parent process and display process id
    cout << "parent process: " << getpid() << " now exiting" << endl; 
    exit(0);
}