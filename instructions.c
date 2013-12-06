#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <time.h>
#include "instructions.h"
#include "stack.h"

// Typedef definitions of the instructionTable and the instructionType's contained within.
typedef struct
{
	// Limit the opcode to 5 char's and the operand to 20 (including the '\0')
	char opcode[6];
	char operand[21];
} instructionType;

typedef struct
{
	instructionType instructions[1024];
	int instructionCount;
} instructionTable;

// Opcodes that require operands
char* opCodes[6] = {"get", "put", "push", "pop", "jf", "j"};

// Instruction table, cannot be accessed outside of this file.
static instructionTable instTab;

// WIC commands
int get(int pc);
int halt();
int push(int pc);
int pop(int pc);
int put(int pc);
int add(int pc);
int sub(int pc);
int mult(int pc);
int divi(int pc);
int and(int pc);
int or(int pc);
int not(int pc);
int tsteq(int pc);
int tstne(int pc);
int tstlt(int pc);
int tstle(int pc);
int tstgt(int pc);
int tstge(int pc);
int j(int pc);
int jf(int pc);

// Function: runInterpreter
// Description: engine that executes the actual method calling from the parsed WIC code
// Params: None
// Returns: None
// Modifies: None
void runInterpreter()
{
	clock_t c0, c1;
	// Set PC to zero
	int pc = 0;
	c0 = clock();
	while (pc != -1)
	{
		// Captures the current Opcode at the PC address
		char* curOp = fetchOpcode(pc);
		// Test found Opcode against our 21 working WIC instructions
		if (strcmp(curOp, "get") == 0)
			pc = get(pc);
		else if (strcmp(curOp, "halt") == 0)
			pc = halt();
		else if (strcmp(curOp, "push") == 0)
			pc = push(pc);
		else if (strcmp(curOp, "put") == 0)
			pc = put(pc);
		else if (strcmp(curOp, "pop") == 0)
			pc = pop(pc);
		else if (strcmp(curOp, "label") == 0)
			pc++;
		else if (strcmp(curOp, "add") == 0)
			pc = add(pc);
		else if (strcmp(curOp, "sub") == 0)
			pc = sub(pc);
		else if (strcmp(curOp, "mult") == 0)
			pc = mult(pc);
		else if (strcmp(curOp, "div") == 0)
			pc = divi(pc);
		else if (strcmp(curOp, "and") == 0)
			pc = and(pc);
		else if (strcmp(curOp, "or") == 0)
			pc = or(pc);
		else if (strcmp(curOp, "not") == 0)
			pc = not(pc);
		else if (strcmp(curOp, "tsteq") == 0)
			pc = tsteq(pc);
		else if (strcmp(curOp, "tstne") == 0)
			pc = tstne(pc);
		else if (strcmp(curOp, "tstlt") == 0)
			pc = tstlt(pc);
		else if (strcmp(curOp, "tstle") == 0)
			pc = tstle(pc);
		else if (strcmp(curOp, "tstgt") == 0)
			pc = tstgt(pc);
		else if (strcmp(curOp, "tstge") == 0)
			pc = tstge(pc);
		else if (strcmp(curOp, "j") == 0)
			pc = j(pc);
		else if (strcmp(curOp, "jf") == 0)
			pc = jf(pc);
		else if (strcmp(curOp, "nop") == 0)
			pc++;
		else if (strlen(curOp) > 0)
		{
			// If line does not contain a valid WIC Opcode and is not blank (or only comments),
			//   the line is un-interpretable and causes the interpreter to exit.
			printf("Error on line %d!\n", pc);
			break;
		}
		else
		{
			// If the end of instructions are reached and no halt is found, restart the program from the
			//   beginning.
			pc = 0;
		}
	}
	c1 = clock();
	printf ("\nElapsed Time:        %f\n", (float) (c1 - c0)/CLOCKS_PER_SEC);
	return;
}

// Function: jf
// Description: If value on the top of the stack is false(0), jump to specified address.
// Params:	PC
// Returns: Incremented PC or address of specified jump address.
// Modifies: None.
int jf(int pc)
{
	int tst = stackPop();
	if (tst == 0)
		// If value on top of stack is false, look up operand in jumpTable and set PC to that address.
		return retrieve(&jumpTable, instTab.instructions[pc].operand);
	else
		return pc + 1;
}

// Function: j
// Description: Jump to the address specified by the symbol specified in the operand
// Params:	PC
// Returns: PC address of specified label from the jump table. 
// Modifies: None.
int j(int pc)
{
	int temp = atoi(instTab.instructions[pc].operand);
	return retrieve(&jumpTable, instTab.instructions[pc].operand);
}

// Function: tsteq
// Description: If value on top of the stack is zero, push '1' on the stack.
// Params:	PC
// Returns: Incremented PC
// Modifies: None.
int tsteq(int pc)
{
	int tst = stackPop();
	if (tst == 0)
		stackPush(1);
	else
		stackPush(0);
	return pc + 1;
}

// Function: tstne
// Description: If value on the top of the stack not equal to zero, push '1' on the stack.
// Params:	PC
// Returns: Incremented PC
// Modifies: None.
int tstne(int pc)
{
	int tst = stackPop();
	if (tst != 0)
		stackPush(1);
	else
		stackPush(0);
	return pc + 1;
}

// Function: tstlt
// Description: If value on the top of the stack is less than zero, push '1' on the stack.
// Params:	PC
// Returns: Incremented PC
// Modifies: None.
int tstlt(int pc)
{
	int tst = stackPop();
	if (tst < 0)
		stackPush(1);
	else
		stackPush(0);
	return pc + 1;
}

// Function: tstle
// Description: If value on the top of the stack is less than or equal to zero, push '1' on the stack.
// Params:	PC
// Returns: Incremented PC
// Modifies: None.
int tstle(int pc)
{
	int tst = stackPop();
	if (tst <= 0)
		stackPush(1);
	else
		stackPush(0);
	return pc + 1;
}

// Function: tstgt
// Description: If value on the top of the stack is greater than zero, push '1' on the stack.
// Params:	PC
// Returns: Incremented PC
// Modifies: None.
int tstgt(int pc)
{
	int tst = stackPop();
	if (tst > 0)
		stackPush(1);
	else
		stackPush(0);
	return pc + 1;
}

// Function: tstge
// Description: If value on the top of the stack is greater than or equal to zero, push '1' on the stack.
// Params:	PC
// Returns: Incremented PC
// Modifies: None.
int tstge(int pc)
{
	int tst = stackPop();
	if (tst >= 0)
		stackPush(1);
	else
		stackPush(0);
	return pc + 1;
}

// Function: and
// Description: pop two values off the stack. If both are non-zero, push '1' on the stack. Otherwise, push '0'
// Params:	PC
// Returns: Incremented PC
// Modifies: None.
int and(int pc)
{
	int x = stackPop();
	int y = stackPop();
	if (x > 0 && y > 0)
		stackPush(1);
	else
		stackPush(0);
	return pc + 1;
}

// Function: not
// Description: Pop value off the stack. If it is '0', push '1', otherwise push '0'.
// Params:	PC
// Returns: Incremented PC
// Modifies: None.
int not(int pc)
{
	int x = stackPop();
	if (x == 0)
		x = 1;
	else
		x = 0;
	stackPush(x);
	return pc + 1;
}

// Function: or
// Description: Pop two values off the stack. If either is non-zero, push a '1' on the stack.
// Params:	PC
// Returns: Incremented PC
// Modifies: None.
int or(int pc)
{
	int x = stackPop();
	int y = stackPop();
	if (x != 0 || y != 0)
		stackPush(1);
	else
		stackPush(0);
	return pc + 1;
}

// Function: mult
// Description: Pop the top two values off the stack and multiply them together. Push the product back onto the stack.
// Params:	PC
// Returns: Incremented PC
// Modifies: None.
int mult(int pc)
{
	int rop = stackPop();
	int lop = stackPop();
	stackPush(rop * lop);
	return pc + 1;
}

// Function: divi
// Description: Pop the top two values off the stack and divide the first by the second. Make sure the divisor does not equal zero
//                before performing the operation, and push the resulting quotient bakc onto the stack. 
// Params:	PC
// Returns: Incremented PC
// Modifies: None.
int divi(int pc)
{
	int rop = stackPop();
	int lop = stackPop();
	if (lop != 0)
		stackPush(lop / rop);
	else
		printf("\nDivide by Zero error on line %d\n", pc);
	return pc + 1;
}

// Function: sub
// Description: Pop the top two values off the stack. Subtract the second from the first and push the result back onto the stack.
// Params:	PC
// Returns: Incremented PC
// Modifies: None.
int sub(int pc)
{
	int rop = stackPop();
	int lop = stackPop();
	stackPush(lop - rop);
	return pc + 1;
}

// Function: add
// Description: Pop the top two values off of the stack. Add them together and push this sum back onto the stack.
// Params:	PC
// Returns: Incremented PC
// Modifies: None.
int add(int pc)
{
	int rop = stackPop();
	int lop = stackPop();
	stackPush(rop + lop);
	return pc + 1;
}

// Function: halt
// Description: Prints 'Halted!', return pc value to halt program execution.
// Params:	PC
// Returns: -1;
// Modifies: None.
int halt()
{
	printf("\nHalted!\n");
	printTables();
	return -1;
}

// Function: push
// Description: Look up operand in symbol table, push it's value onto the stack. If operand is an immediate value (integer), go
//                ahead and push it onto the stack. 
// Params:	PC
// Returns: Incremented PC
// Modifies: stack.
int push(int pc)
{
	char* pushed = instTab.instructions[pc].operand;
	int val;
	// 
	if (isdigit(pushed[0]))
	{
		int temp = atoi(instTab.instructions[pc].operand);
		stackPush(temp);
		return pc + 1;
	}
	val = retrieve(&symbolTable, pushed);
	stackPush(val);
	return pc + 1;
}

// Function: pop
// Description: Pop top value off of the stack, save this value to specified operand in the symbol table.
// Params:	PC
// Returns: Incremented PC
// Modifies: stack, symbol table. 
int pop(int pc)
{
	int popped = stackPop();
	store(&symbolTable, popped, instTab.instructions[pc].operand);
	return pc + 1;
}

// Function: put
// Description: Look up operand in symbol table, display(print) its value to the screen.
// Params:	PC
// Returns: Incremented PC
// Modifies: None.
int put(int pc)
{
	printf("%s = %d\n", instTab.instructions[pc].operand, retrieve(&symbolTable, instTab.instructions[pc].operand));
	return pc + 1;
}

// Function: get
// Description: Capture user input and save it to the specified symbol table key.
// Params:	PC
// Returns: Incremented PC
// Modifies: Symbol table.
int get(int pc)
{
	int user;
	printf("Enter %s > ", instTab.instructions[pc].operand);
	scanf("%d", &user);
	store(&symbolTable, user, instTab.instructions[pc].operand);
	return pc + 1;
}

// Function: Initialize
// Description: Set up the stack, jump/symbol tables before beginning program execution.
// Params: None
// Returns: None
// Modifies: Stack, Symbol/Jump tables.
void initialize()
{
	instTab.instructionCount = 0;
	initStack();
	initializeTable(&jumpTable);
	initializeTable(&symbolTable);
}

// Function: printInstTable
// Description: Prints the instruction table. Number in parentheses is the address of the instruction, followed by
//                the Opcode and Operand at that address.
// Params: None.
// Returns: None.
// Modifies: None.
void printInstTable()
{
	int i;
	for (i = 0; i < instTab.instructionCount; i++)
	{
		printf("(%d) %s (%s) \n", i, instTab.instructions[i].opcode, instTab.instructions[i].operand);
	}
	printf("\n");
}

// Function: printTables
// Description: Prints out the jump/symbol tables, displaying their contents.
// Params: None.
// Returns: None.
// Modifies: None.
void printTables()
{
	int i;
	printf("\nJump Tables Values: \n");
	for(i = 0; i < jumpTable.size; i++)
	{
		printf("Label: <%s>, Address: <%d>\n", jumpTable.entries[i].key, jumpTable.entries[i].value);
	}
	printf("\nSymbol Table Values: \n");
	for (i = 0; i < symbolTable.size; i++)
	{
		printf("Symbol: <%s>, Value: <%d>\n", symbolTable.entries[i].key, symbolTable.entries[i].value);
	}
}

// Function: hasOperand
// Description: Checks input string against the 6 operations that do need an operand. If an operand is needed for that instruction
//                 the function returns 1. Otherwise it returns 0.
// Params: String opcode.
// Returns: '1' or '0' depending on whether the parameter string opcode has an operand.
// Modifies: None.
int hasOperand(char * opcode)
{
	int i;
	for (i = 0; i < 6; i++)
	{
		if(strcmp(opcode, opCodes[i]) == 0)
		{
			return 1;
		}
	}
	return 0;
}

// Function: insertInstruction
// Description: Given an address, opcode, and operand, this function inserts the resulting WIC instruction into the instruction table.
// Params: Address of instruction, Opcode, Operand
// Returns: None.
// Modifies: None.
void insertInstruction(int address, char* op, char* ope)
{
	instructionType inst;
	strcpy(inst.opcode, op);
	strcpy(inst.operand, ope);
	instTab.instructions[address] = inst;
	instTab.instructionCount++;
	return;
}

// Function: fetchOpcode
// Description: Given an address passed in, this function returns the corresponding opcode found at that address in the instruction table.
// Params: Address.
// Returns: String opcode.
// Modifies: None.
char* fetchOpcode(int address)
{
	return instTab.instructions[address].opcode;
}