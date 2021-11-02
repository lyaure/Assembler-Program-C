#ifndef INSTRUCTION_CODE_H
#define INSTRUCTION_CODE_H


#include "globals.h"


struct  code{
	int index;

	struct word{
		unsigned int dest  : 2;
		unsigned int src   : 2;
		unsigned int funct : 4;
		unsigned int opcode: 4;
		unsigned int unused: 20;
	
	}word;

	char attribute;

	char *info;

	struct code *next;
};

typedef struct code InstructionCode;

void insertCode(InstructionCode **head, int IC, int dest, int src, int funct, int opcode, char attribute, char *information);

void completeWord(InstructionCode **head, int index, int dest, int src, int funct, int opcode, char attribute);

void join(InstructionCode **head1, InstructionCode **head2);

void freeCodeTable(InstructionCode **head);


#endif