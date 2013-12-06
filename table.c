#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "table.h"

void initializeTable(tableType *Xtable)
{
	// Just initializes the table size to zero upon intial creation (in main).
	Xtable -> size = 0;
}

// Function: store
// Description: Saves a passed in key and value to the passed in table.
// Params: Table to store to, value to store, key to store at.
// Returns: None.
// Modifies: Table passed in.
void store(tableType *Xtable, int val, char *k)
{
	// If the specified key is already present in the table, modify
	// its value instead of creating a duplicate.
	if (retrieve(Xtable, k) >= 0)
	{
		int i;
		for (i = 0; i < Xtable -> size; i++)
		{
			if (strncmp(Xtable -> entries[i].key, k, 2) == 0)
			{
				Xtable -> entries[i].value = val;
			}
		}
		return;
	}
	// Otherwise store whole entry in the specified table
	{
		// Create temporary tableEntry given parameter data
		tableEntry temp;
		// Must always use char arrays vs. pointers - those are immutable in C. 
		strcpy(temp.key, k);
		temp.value = val;
		Xtable -> entries[Xtable -> size] = temp;
		Xtable -> size++;
		return;
	}
	
}

// Function: retrieve
// Description: Retrieves the value in the passed in table corresponding to the passed in key.
// Params: Table to search, key to look for.
// Returns: Value of key if found, -1 otherwise.
// Modifies: Passed in table.
int retrieve(tableType *Xtable, char* k)
{
	int i;
	int ret;
	ret = -1;
	// Loop to check if key already exists 
	for (i = 0; i < Xtable -> size; i++)
	{
		if (strncmp(Xtable -> entries[i].key, k, 3) == 0)
		{
			ret = Xtable -> entries[i].value;
		}
	}
	return ret;
}