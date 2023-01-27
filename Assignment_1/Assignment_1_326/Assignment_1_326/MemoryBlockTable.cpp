#pragma once

#include "stdafx.h"
#include "MemoryBlockTable.h"

/*
contrustor for the MemoryBlockTable prototype
instantiates 1024 blocks for memory and uses the first 32 for system
*/
MemoryBlockTable::MemoryBlockTable()
{
	blocksAvailable = 1024;
	
	for (int i = 0; i < 32; i++)
	{
		memory[i] = true;
		blocksAvailable--;
	}

	cout << "Memory Block Table Created." << endl;
}

/*
destructor for MemoryBlockTabled
*/
MemoryBlockTable::~MemoryBlockTable()
{
	cout << "Memory Block Table Destroyed." << endl;
}

/*
sets available blocks from overloaded data member int val
*/
int MemoryBlockTable::getAvailableBlocks()
{
	return blocksAvailable;
}

/*
sets available blocks from overloaded data member int val
*/
void MemoryBlockTable::setAvailableBlocks(int val)
{
	blocksAvailable = val;
}

/*
allocates memory in the MemoryBlockTable for the page table, takes in pointer to ProcessControlBlock pageTable
stores MemoryBlock table location and stores it in the ProcessControlBlock's pagetable
*/
void MemoryBlockTable::allocMemory(int * pageTable, int size)
{
	cout << "Current Page Table: " << endl;

	for (int i = 0, j = 0; i < 1024 && j < size; i++)
	{
		if (memory[i] == false)
		{
			pageTable[j] = i; //find and initialize memory location
			cout << setw(3);
			cout << "[" << pageTable[j] << "]";
			j++; //increment pagetable counter
			memory[i] = true; //memory to allocated
		}
	}
	cout << endl;
}

/*
deallocates the MemoryBlockTable where the ProcessControlBlock's pagetable was using a pointer to the ProcessControlBlock's pagetable
adds blocksAvailable back depending on size
*/
void MemoryBlockTable::deallocMemory(int * pageTable, int size)
{
	for (int i = 0; i < size; i++)
	{
		memory[pageTable[i]] = false; //free memory block
	}
	blocksAvailable = blocksAvailable + size; //update blocks available counter
}

/*
displays the MemoryBlockTable
*/
void MemoryBlockTable::displayMemory()
{
	cout << "Current Memory Block Table: " << endl;
	for (int i = 0; i < 1024; i++)
	{
		cout << setw(2);
		cout << "[" << memory[i] << "]";
	}
	cout << endl;
}
