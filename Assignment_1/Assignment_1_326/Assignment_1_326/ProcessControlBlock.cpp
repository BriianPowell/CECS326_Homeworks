#pragma once

#include "stdafx.h"
#include "ProcessControlBlock.h"

/*
instantiates a ProcessControlBlock with an id and size
int id - unique process id
size - size of the process (pageTable)
*/
ProcessControlBlock::ProcessControlBlock(int id, int size)
{
	pid = id;
	processSize = size;
	pageTable = NULL;

	cout << "Process Control Block [" << pid << "] created." << endl;
}

/*
desctructor for the ProcessControlBlock
deletes the pageTable for each process
*/
ProcessControlBlock::~ProcessControlBlock()
{
	delete[] pageTable;

	cout << "Process Control Block [" << pid << "] destroyed." << endl;
}

/*
returns unique PID of the process
*/
int ProcessControlBlock::getPID()
{
	return pid;
}

/*
returns size of the process
*/
int ProcessControlBlock::getSize()
{
	return processSize;
}

/*
returns pageTable of the process
*/
int * ProcessControlBlock::getPageTable()
{
	return pageTable;
}

/*
creates a pageTable array of type int for the process
int size - size of the array to be made
*/
void ProcessControlBlock::setPageTable(int size)
{
	pageTable = new int[size];
}
