/*
Brian Powell
CECS 326 - Shui Lam
Assignment 1 

This assignment serves as a review of C/C++ programming that includes dynamic memory allocation, pointer, 
struct, queue, array and random number generation. The program is to implement much simplified operations 
for part of the process and memory management in operating systems.
*/

#include "stdafx.h"
#include "MemoryBlockTable.h"
#include "ProcessControlBlock.h"
#include <iostream>
#include <vector>
#include <string>
#include <ctime>


using namespace std;

//Function Prototypes	
int menu();
void initiateProcess();
void printReadyQueue();
void terminateProcess();
void exitProg();

//Global Variables
MemoryBlockTable MBT;
vector<ProcessControlBlock *> ReadyQueue; //Needs to be a vector so that size can dynamically change
int pidcounter = 0; //process counter

/*
main() calls the menu() function and acts on the value returned from menu().
The switch then figures out where to go in the program.
*/
int main()
{
	bool exittoken = false;
	 
	while (!exittoken)
	{
		switch (menu())
		{
		case 1:
		{
			initiateProcess();
			break;
		}
		case 2:
		{
			printReadyQueue();
			break;
		}
		case 3:
		{
			terminateProcess();
			break;
		}
		case 4:
		{
			exittoken = true;
			exitProg();
			cout << "Exiting Program." << endl;
			cout << "Good bye." << endl;
			system("PAUSE");
			break;
		}
		default:
			cout << "Invalid choice." << endl;
			break;
		}
	}

    return 0;
}

/*
Menu() displays a menu and checks for invalid input then returns an integer to main().
*/
int menu()
{
	int choice;
	bool validInput = false;

	while (!validInput)
	{
		validInput = true;
		cout << "\n__________________________________________" << endl;
		cout << "| Please Make a Choice:                  |" << endl;
		cout << "|========================================|" << endl;
		cout << "| Initiate a Process               [1]   |" << endl;
		cout << "| Print Processes in Ready Queue   [2]   |" << endl;
		cout << "| Terminatee Process by ID         [3]   |" << endl;
		cout << "|                                        |" << endl;
		cout << "| Exit                             [4]   |" << endl;
		cout << "|========================================|" << endl;
		cout << "Selection: ";
		cin >> choice;

		if (cin.fail()) //cin.fail() to make sure that value is the correct type
		{
			cin.clear();
			cin.ignore();
			cout << "Please enter an integer value." << endl;
			validInput = false;
		}
	}

	return choice;
}

/*
Initiates a Process Control Block then allocates enough memory in the Memory Control Block for it.
*/
void initiateProcess()
{
	cout << endl << "Memory Size: " << MBT.getAvailableBlocks() << endl;

	srand(time(0)); //seed the rand function
	int size = rand() % 150 + 10; //Generate a random value from 10 to 150

	//Dyanamically create a new Process Control Block
	ProcessControlBlock * PCB = new ProcessControlBlock(pidcounter, size);
	if (MBT.getAvailableBlocks() - size >= 0) //making sure there is memory available
	{
		pidcounter++; //increment process ID
		PCB->setPageTable(size); //setting page table size from rand() function 
		MBT.allocMemory(PCB->getPageTable(), PCB->getSize()); //alloc memory in MBT to store the PCB
		ReadyQueue.push_back(PCB); //insert PCB into the ReadyQueue

		MBT.displayMemory(); //display MBT
		MBT.setAvailableBlocks(MBT.getAvailableBlocks() - size); //update available blocks
		cout << "Updated Memory Space: [" << MBT.getAvailableBlocks() << "] " << endl;
	}
	else //error if PCB doesn't fit in MBT
	{
		cout << endl << "[Error]\nInsufficient free blocks in Memory Block Table." << endl;
		cout << "Required blocks for Process Control Block: [" << size << "]." << endl;
		cout << "Available Memory Block Table blocks: [" << MBT.getAvailableBlocks() << "]." << endl;
		delete PCB; //PCB won't fit, delete it to make another and try again
	}
}

/*
Prints all Processes in the ready queue as well as their size.
*/
void printReadyQueue()
{
	cout << "\nReady Queue:";
	//Iterate through vector and display all PIDS and size
	for (int i = 0; i < ReadyQueue.size(); i++)
	{
		ProcessControlBlock * temp = ReadyQueue.at(i);
		cout << "\nProcess Control Block ID: " << temp->getPID() << "\nSize: [" << temp->getSize() << "] blocks. " << endl;
	}
}

/*
Process ID selected by user will be terminated
*/
void terminateProcess()
{
	int PIDchoice;
	bool foundPID = false;

	//Iterate through vector and display all PIDS and size
	cout << "\nReady Queue:\nProcess Control IDs: ";
	if (!ReadyQueue.empty())
	{
		for (int i = 0; i < ReadyQueue.size(); i++)
		{
			ProcessControlBlock * temp = ReadyQueue.at(i);
			cout << setw(2);
			cout << "[" << temp->getPID() << "]";
		}
		cout << endl;
		cout << "Select a Process ID to terminate: ";
		cin >> PIDchoice;
		cout << endl;
	}
	else
	{
		cout << "\n\n[Ready Queue is empty.]" << endl;
		cout << "[Please initiate a process.]" << endl;
		main();
	}

	//Iterate through list to find PID
	for (int i = 0; i < ReadyQueue.size(); i++)
	{
		ProcessControlBlock * temp = &(*ReadyQueue[i]); //reference to data being pointed to, this is each individual PCB in ReadyQueue
		if(temp->getPID() == PIDchoice)
		{
			foundPID = true;
			MBT.deallocMemory(temp->getPageTable(), temp->getSize()); //deallocate memory to be used again
			delete temp;
			ReadyQueue.erase(ReadyQueue.begin() + i); //remove Process Block from Ready Queue
		}
	}

	//Display error for a PID that is not found
	if (!foundPID)
	{
		cout << "[Error]\nProcess ID [" << PIDchoice << "] was not found." << endl;
		cout << "Please Enter a Valid Process ID.";
		terminateProcess();
	}
}

/*
Deallocates all PCB in the ReadyQueue and exits
*/
void exitProg()
{
	string choice = "";

	//Make sure that exit is wanted even if Ready Queue is populated
	if (!ReadyQueue.empty())
	{
		cout << "\nReady Queue is not empty." << endl;
		cout << "Continue anyway?[Y|N]" << endl;
		cin >> choice;
		cin.ignore();

		if (choice == "y" || choice == "yes")
		{
			for (int i = 0; i < ReadyQueue.size(); i++)
			{
				ProcessControlBlock * temp = &(*ReadyQueue[i]); //Temporary variable to access PCB
				delete temp; //Delete PCB
			}
		}
		else
		{
			cout << "\nReturning to menu." << endl;
			main();
		}
	}
	else
	{
		for (int i = 0; i < ReadyQueue.size(); i++)
		{
			ProcessControlBlock * temp = &(*ReadyQueue[i]); //Temporary variable to access PCB
			delete temp; //Delete PCB
		}
	}

}
