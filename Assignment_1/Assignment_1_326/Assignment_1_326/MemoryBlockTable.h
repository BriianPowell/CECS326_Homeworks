#pragma once

#include "stdafx.h"
#include <iostream>
#include <iomanip>

using namespace std;

/*
MemoryBlockTable();
	contrustor for the MemoryBlockTable prototype
	instantiates 1024 blocks for memory and uses the first 32 for system

~MemoryBlockTable();
	destructor for MemoryBlockTable

int getAvailableBlocks();
	returns blocksAvailable

void setAvailableBlocks(int val);
	sets available blocks from overloaded data member int val

void allocMemory(int * pageTable, int size);
	allocates memory in the MemoryBlockTable for the page table, takes in pointer to ProcessControlBlock pageTable
	stores MemoryBlock table location and stores it in the ProcessControlBlock's pagetable

void deallocMemory(int * pageTable, int size);
	deallocates the MemoryBlockTable where the ProcessControlBlock's pagetable was using a pointer to the ProcessControlBlock's pagetable
	adds blocksAvailable back depending on size

void displayMemory();
	displays the MemoryBlockTable

int blocksAvailable
	stores available blocks number of the MemoryBlockTable

bool memory[1024]
	holds a bool array for the MemoryBlockTable
	true = block is allocated to a pageTable
*/

class MemoryBlockTable
{
public:
	MemoryBlockTable();
	~MemoryBlockTable();
	int getAvailableBlocks();
	void setAvailableBlocks(int val);
	void allocMemory(int * pageTable, int size);
	void deallocMemory(int * pageTable, int size);
	void displayMemory();

private:
	int blocksAvailable;
	bool memory[1024];
};