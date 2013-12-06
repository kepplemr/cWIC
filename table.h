#ifndef TABLE_H
#define TABLE_H

typedef struct 
{
	char key[6];
	int value;
} tableEntry;

typedef struct 
{
	// Limit table entries to 20
	tableEntry entries[20];
	int size;
} tableType;

// Declaration of the jump/symbol tables
const tableType symbolTable;
const tableType jumpTable;

void initializeTable(tableType *Xtable);
void store(tableType *Xtable, int val, char* k);
int retrieve(tableType *Xtable, char* key);

#endif
