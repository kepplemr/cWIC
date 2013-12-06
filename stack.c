#include "stack.h"

// Typedef definition of the stack struct. Since no other files need access to this data type, this struct definition
// is not needed in the header.
typedef struct
{
	int theStack[20];
	int stackIndex;
} stack;

// The stack we'll use
stack Stack;

void initStack()
{
	// Initialize stack index to -1 so that first push will increment it to zero. It also prevents pop from
	// popping anything off until something is pushed on initially.
	Stack.stackIndex = -1;
}

// Function: stackPush
// Description: Pushes the passed in parameter onto the stack.
// Params: Value to be pushed onto the stack.
// Returns: None.
// Modifies: Stack.
void stackPush(int x)
{
	Stack.stackIndex++;
	Stack.theStack[Stack.stackIndex] = x;
}

// Function: stackPop
// Description: Pop's the top value off of the stack and returns it.
// Params: None.
// Returns: Top value on the stack.
// Modifies: Stack.
int stackPop()
{
	int temp;
	if (Stack.stackIndex >= 0)
	{
		temp = Stack.theStack[Stack.stackIndex];
		Stack.stackIndex--;
	}
	return temp;
}