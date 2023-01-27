#pragma once

#include "stdafx.h"
#include <iostream>

using namespace std;

/*
ProcessControlBlock(int id, int size);
	instantiates a ProcessControlBlock with an id and size
	int id - unique process id
	size - size of the process (pageTable)

~ProcessControlBlock();
	desctructor for the ProcessControlBlock
	deletes the pageTable for each process

int getPID();
	returns unique PID of the process

int getSize();
	returns size of the process

int *getPageTable();
	returns pageTable of the process

void setPageTable(int size);
	creates a pageTable array of type int for the process
	int size - size of the array to be made

int pid
	unique identifier of each process

int processSize
	size of the process

int * pageTable
	points to data location of the pageTable array
*/
class ProcessControlBlock
{
public:
	ProcessControlBlock(int id, int size);
	~ProcessControlBlock();
	int getPID();
	int getSize();
	int *getPageTable();
	void setPageTable(int size);


private:
	int pid;
	int processSize;
	int * pageTable;

};
