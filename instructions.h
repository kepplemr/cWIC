#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include "table.h"
#include "stack.h"

// Outside accessible functions in instructions.c
void runInterpreter();
void printTables();
void printInstTable();
void initialize();
int hasOperand(char * opcode);
void insertInstruction(int address, char* opcode, char* operand);
char* fetchOpcode(int address);

#endif