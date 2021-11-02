#ifndef INSTRUCTIONS_TABLE_H
#define INSTRUCTIONS_TABLE_H

#include "globals.h"


#define NUM_OF_INSTRUCTIONS 16 
struct instruction{
	char *name;
	int opcode;
	int funct;
	int operandsNumber;

	struct srcAddressMode{
		unsigned int immediate	:1;
		unsigned int direct 		:1;
		unsigned int relative	:1;
		unsigned int regDirect	:1;
	}srcAddressMode;

	struct destAddressMode{
		unsigned int immediate	:1;
		unsigned int direct 		:1;
		unsigned int relative	:1;
		unsigned int regDirect	:1;
	}destAddressMode;
};

typedef struct instruction Instruction;

enum addressingMode {IMMEDIATE, DIRECT, RELATIVE, REGISTER_DIRECT};
enum operandName{SRC, DEST};

Instruction* initInstructionsTable();

int getOperandsNumber(Instruction *table, char* instruction);
int getInstruction(Instruction *table, char *name);
int checkAddressingModeCompatibility(int mode, Instruction *table, int instructionNumber, int opNumber);

#endif