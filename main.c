#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instructions.h"
#include "stack.h"
#include "table.h"

FILE* getFile(char* extension, char* input);
void getInstFromFile(FILE* file);
void printPreProcessed();
char* discardLine(char * line);

// Function: Main program fucntion
// Description: Macro-level control function for the WIC interpreter.
// Params: None
// Returns: 0 upon successful completion.
// Modifies: None.
int main()
{
    // Char pointer passed into function, otherwise local variable allocation on the stack gets messed
	// up by the various outside function calls - strtok, strcmp, printf, etc. Could make them const, extern, or m/calloc -
	// but this is easier.
	char* a;
	char* b;
	FILE* file = getFile(a, b);
	// Initialize tables.
	initialize();
	// Open file, parse WIC code
	getInstFromFile(file);
	// Print out after pre-processing
	printPreProcessed();
	// Run the WIC code
	runInterpreter();
	return 0;
}

// Function: PrintPreProcessed - prints the pre-processed WIC.
// Description: Calls functions in instructions.c to print the Instruction table as well as the jump/symbol
//                tables afterwards.
// Params: None
// Returns: None
// Modifies: None
void printPreProcessed()
{
	printInstTable();
}

// Function: getInstFromFile
// Description: Opens the passed in file pointer, captures each line of WIC commands and passes the corresponding
//                instruction off to be added to the instruction table.
// Params: pointer to file (FILE* file)
// Returns: None.
// Modifies: jumpTable, instTable
void getInstFromFile(FILE* file)
{
	static char* op;
	static char* operand;
	char currentLine[120];
	int address = 0;
	while (fgets(currentLine, 120, file) != NULL)
	{
		// Make sure to reset the op/operand strings on every newline
		op = "";
		operand = "";
		// Op = first word before ' ' 
		op = strtok(currentLine, " ");
		op = discardLine(op);
		// Operand = word after ' '
		operand = strtok(NULL, " ");
		// If operand doesn't exist, don't pass in NULL
		if (operand == NULL)
		{
			operand = "";
		}
		if (hasOperand(op) == 1)
		{
			operand = discardLine(operand);
			insertInstruction(address, op, operand);
		}
		// Strncmp must be used rather than strcmp to avoid buffer overflow errors
		else if (strncmp(operand, "label", 5) == 0)
		{
			insertInstruction(address, "label", op);
			// Insert the op (operand when we display) into the jumpTable
			store(&jumpTable, address, op);
		}
		else if (strcmp(op, "") == 0)
		{
			// If there is nothing found on a line, address is a no-op, but shouldn't crash
			insertInstruction(address, "nop", op);
		}
		else
		{
			operand = "";
			insertInstruction(address, op, operand);
		}
		address++;
	}
	fflush(file);
}

// Function: discardLine
// Description: trims the inputted code line down, removing unnecessary comments and whitespace.
// Params: String line (char* line)
// Returns: String line (char* line) - after trimming
// Modifies: None
char* discardLine(char * line)
{
	// No need for interpreted code to display end of line comments, etc.
	// So get rid of it and also trim out the useless whitespace.
	int length = strlen(line);
	int i = 0;
	while (i < length && line[i] != ' ' && line[i] != '\n' && line[i] != '|')
	{
		line[i] = line[i];
		i++;
	}
	line[i] = '\0';
	return line;
}

// Function: getFile
// Description: prompts for user input, tests for correct '.wic' extension, attempts to open file and establish file pointer.
// Params: Two empty string inputs for modifying.
// Returns: File pointer to user-inputted file (FILE* file)
// Modifies: Two passed in strings
FILE* getFile(char* extension, char* input)
{
	FILE *file;
	// Need to malloc fileName memory so it also doesn't get wiped during all the coming outside function calls.
	char* fileName = (char*) malloc(sizeof(char) * 30);
	printf("***     Welcome to Michael Kepple's WREN Interpreter    ***\n");
	printf("*** Please enter the name of the .wic file to interpret ***\n");
	printf("> ");
	scanf("%s", input); 
	// Copy user input over to the allocated char* string to use later
	memcpy(fileName, input, sizeof(char) * 30);
	input = strtok(input, ".");
	extension = strtok(NULL, ".");
	// Check to ensure extension is .wic
	if (strcmp(extension, "wic") != 0)
	{
		printf("You have entered an invalid filename - WIC files must end in '.wic'!\n");
		exit(1);
	}
	// Attempt to open the specified .wic file
	file = fopen(fileName, "r");
	if (file == NULL)
	{
		printf("The file could not be loaded - please check the name!\n");
		exit(2);
	}
	return file;
}
