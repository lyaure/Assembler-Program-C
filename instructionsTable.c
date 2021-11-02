#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instructionsTable.h"


Instruction* initInstructionsTable(){
	static Instruction instructionsTable[NUM_OF_INSTRUCTIONS];
	
	instructionsTable[0].name = "mov";
	instructionsTable[0].opcode = 0;
	instructionsTable[0].funct = 0;
	instructionsTable[0].operandsNumber = 2;
	instructionsTable[0].srcAddressMode.immediate = 1;
	instructionsTable[0].srcAddressMode.direct = 1;
	instructionsTable[0].srcAddressMode.relative = 0;
	instructionsTable[0].srcAddressMode.regDirect = 1;
	instructionsTable[0].destAddressMode.immediate = 0;
	instructionsTable[0].destAddressMode.direct = 1;
	instructionsTable[0].destAddressMode.relative = 0;
	instructionsTable[0].destAddressMode.regDirect = 1;


	instructionsTable[1].name = "cmp";
	instructionsTable[1].opcode = 1;
	instructionsTable[1].funct = 0;
	instructionsTable[1].operandsNumber = 2;
	instructionsTable[1].srcAddressMode.immediate = 1;
	instructionsTable[1].srcAddressMode.direct = 1;
	instructionsTable[1].srcAddressMode.relative = 0;
	instructionsTable[1].srcAddressMode.regDirect = 1;
	instructionsTable[1].destAddressMode.immediate = 1;
	instructionsTable[1].destAddressMode.direct = 1;
	instructionsTable[1].destAddressMode.relative = 0;
	instructionsTable[1].destAddressMode.regDirect = 1;



	instructionsTable[2].name = "add";
	instructionsTable[2].opcode = 2;
	instructionsTable[2].funct = 10;
	instructionsTable[2].operandsNumber = 2;
	instructionsTable[2].srcAddressMode.immediate = 1;
	instructionsTable[2].srcAddressMode.direct = 1;
	instructionsTable[2].srcAddressMode.relative = 0;
	instructionsTable[2].srcAddressMode.regDirect = 1;
	instructionsTable[2].destAddressMode.immediate = 0;
	instructionsTable[2].destAddressMode.direct = 1;
	instructionsTable[2].destAddressMode.relative = 0;
	instructionsTable[2].destAddressMode.regDirect = 1;

	instructionsTable[3].name = "sub";
	instructionsTable[3].opcode = 2;
	instructionsTable[3].funct = 11;
	instructionsTable[3].operandsNumber = 2;
	instructionsTable[3].srcAddressMode.immediate = 1;
	instructionsTable[3].srcAddressMode.direct = 1;
	instructionsTable[3].srcAddressMode.relative = 0;
	instructionsTable[3].srcAddressMode.regDirect = 1;
	instructionsTable[3].destAddressMode.immediate = 0;
	instructionsTable[3].destAddressMode.direct = 1;
	instructionsTable[3].destAddressMode.relative = 0;
	instructionsTable[3].destAddressMode.regDirect = 1;

	instructionsTable[4].name = "lea";
	instructionsTable[4].opcode = 4;
	instructionsTable[4].funct = 0;
	instructionsTable[4].operandsNumber = 2;
	instructionsTable[4].srcAddressMode.immediate = 0;
	instructionsTable[4].srcAddressMode.direct = 1;
	instructionsTable[4].srcAddressMode.relative = 0;
	instructionsTable[4].srcAddressMode.regDirect = 0;
	instructionsTable[4].destAddressMode.immediate = 0;
	instructionsTable[4].destAddressMode.direct = 1;
	instructionsTable[4].destAddressMode.relative = 0;
	instructionsTable[4].destAddressMode.regDirect = 1;

	instructionsTable[5].name = "clr";
	instructionsTable[5].opcode = 5;
	instructionsTable[5].funct = 10;
	instructionsTable[5].operandsNumber = 1;
	instructionsTable[5].srcAddressMode.immediate = 0;
	instructionsTable[5].srcAddressMode.direct = 0;
	instructionsTable[5].srcAddressMode.relative = 0;
	instructionsTable[5].srcAddressMode.regDirect = 0;
	instructionsTable[5].destAddressMode.immediate = 0;
	instructionsTable[5].destAddressMode.direct = 1;
	instructionsTable[5].destAddressMode.relative = 0;
	instructionsTable[5].destAddressMode.regDirect = 1;

	instructionsTable[6].name = "not";
	instructionsTable[6].opcode = 5;
	instructionsTable[6].funct = 11;
	instructionsTable[6].operandsNumber = 1;
	instructionsTable[6].srcAddressMode.immediate = 0;
	instructionsTable[6].srcAddressMode.direct = 0;
	instructionsTable[6].srcAddressMode.relative = 0;
	instructionsTable[6].srcAddressMode.regDirect = 0;
	instructionsTable[6].destAddressMode.immediate = 0;
	instructionsTable[6].destAddressMode.direct = 1;
	instructionsTable[6].destAddressMode.relative = 0;
	instructionsTable[6].destAddressMode.regDirect = 1;

	instructionsTable[7].name = "inc";
	instructionsTable[7].opcode = 5;
	instructionsTable[7].funct = 12;
	instructionsTable[7].operandsNumber = 1;
	instructionsTable[7].srcAddressMode.immediate = 0;
	instructionsTable[7].srcAddressMode.direct = 0;
	instructionsTable[7].srcAddressMode.relative = 0;
	instructionsTable[7].srcAddressMode.regDirect = 0;
	instructionsTable[7].destAddressMode.immediate = 0;
	instructionsTable[7].destAddressMode.direct = 1;
	instructionsTable[7].destAddressMode.relative = 0;
	instructionsTable[7].destAddressMode.regDirect = 1;

	instructionsTable[8].name = "dec";
	instructionsTable[8].opcode = 5;
	instructionsTable[8].funct = 13;
	instructionsTable[8].operandsNumber = 1;
	instructionsTable[8].srcAddressMode.immediate = 0;
	instructionsTable[8].srcAddressMode.direct = 0;
	instructionsTable[8].srcAddressMode.relative = 0;
	instructionsTable[8].srcAddressMode.regDirect = 0;
	instructionsTable[8].destAddressMode.immediate = 0;
	instructionsTable[8].destAddressMode.direct = 1;
	instructionsTable[8].destAddressMode.relative = 0;
	instructionsTable[8].destAddressMode.regDirect = 1;

	instructionsTable[9].name = "jmp";
	instructionsTable[9].opcode = 9;
	instructionsTable[9].funct = 10;
	instructionsTable[9].operandsNumber = 1;
	instructionsTable[9].srcAddressMode.immediate = 0;
	instructionsTable[9].srcAddressMode.direct = 0;
	instructionsTable[9].srcAddressMode.relative = 0;
	instructionsTable[9].srcAddressMode.regDirect = 0;
	instructionsTable[9].destAddressMode.immediate = 0;
	instructionsTable[9].destAddressMode.direct = 1;
	instructionsTable[9].destAddressMode.relative = 1;
	instructionsTable[9].destAddressMode.regDirect = 0;

	instructionsTable[10].name = "bne";
	instructionsTable[10].opcode = 9;
	instructionsTable[10].funct = 11;
	instructionsTable[10].operandsNumber = 1;
	instructionsTable[10].srcAddressMode.immediate = 0;
	instructionsTable[10].srcAddressMode.direct = 0;
	instructionsTable[10].srcAddressMode.relative = 0;
	instructionsTable[10].srcAddressMode.regDirect = 0;
	instructionsTable[10].destAddressMode.immediate = 0;
	instructionsTable[10].destAddressMode.direct = 1;
	instructionsTable[10].destAddressMode.relative = 1;
	instructionsTable[10].destAddressMode.regDirect = 0;

	instructionsTable[11].name ="jsr";
	instructionsTable[11].opcode = 9;
	instructionsTable[11].funct = 12;
	instructionsTable[11].operandsNumber = 1;
	instructionsTable[11].srcAddressMode.immediate = 0;
	instructionsTable[11].srcAddressMode.direct = 0;
	instructionsTable[11].srcAddressMode.relative = 0;
	instructionsTable[11].srcAddressMode.regDirect = 0;
	instructionsTable[11].destAddressMode.immediate = 0;
	instructionsTable[11].destAddressMode.direct = 1;
	instructionsTable[11].destAddressMode.relative = 1;
	instructionsTable[11].destAddressMode.regDirect = 0;

	instructionsTable[12].name = "red";
	instructionsTable[12].opcode = 12;
	instructionsTable[12].funct = 0;
	instructionsTable[12].operandsNumber = 1;
	instructionsTable[12].srcAddressMode.immediate = 0;
	instructionsTable[12].srcAddressMode.direct = 0;
	instructionsTable[12].srcAddressMode.relative = 0;
	instructionsTable[12].srcAddressMode.regDirect = 0;
	instructionsTable[12].destAddressMode.immediate = 0;
	instructionsTable[12].destAddressMode.direct = 1;
	instructionsTable[12].destAddressMode.relative = 0;
	instructionsTable[12].destAddressMode.regDirect = 1;

	instructionsTable[13].name = "prn";
	instructionsTable[13].opcode = 13;
	instructionsTable[13].funct = 0;
	instructionsTable[13].operandsNumber = 1;
	instructionsTable[13].srcAddressMode.immediate = 0;
	instructionsTable[13].srcAddressMode.direct = 0;
	instructionsTable[13].srcAddressMode.relative = 0;
	instructionsTable[13].srcAddressMode.regDirect = 0;
	instructionsTable[13].destAddressMode.immediate = 1;
	instructionsTable[13].destAddressMode.direct = 1;
	instructionsTable[13].destAddressMode.relative = 0;
	instructionsTable[13].destAddressMode.regDirect = 1;

	instructionsTable[14].name = "rts";
	instructionsTable[14].opcode = 14;
	instructionsTable[14].funct = 0;
	instructionsTable[14].operandsNumber = 0;
	instructionsTable[14].srcAddressMode.immediate = 0;
	instructionsTable[14].srcAddressMode.direct = 0;
	instructionsTable[14].srcAddressMode.relative = 0;
	instructionsTable[14].srcAddressMode.regDirect = 0;
	instructionsTable[14].destAddressMode.immediate = 0;
	instructionsTable[14].destAddressMode.direct = 0;
	instructionsTable[14].destAddressMode.relative = 0;
	instructionsTable[14].destAddressMode.regDirect = 0;

	instructionsTable[15].name = "stop";
	instructionsTable[15].opcode = 15;
	instructionsTable[15].funct = 0;
	instructionsTable[15].operandsNumber = 0;
	instructionsTable[15].srcAddressMode.immediate = 0;
	instructionsTable[15].srcAddressMode.direct = 0;
	instructionsTable[15].srcAddressMode.relative = 0;
	instructionsTable[15].srcAddressMode.regDirect = 0;
	instructionsTable[15].destAddressMode.immediate = 0;
	instructionsTable[15].destAddressMode.direct = 0;
	instructionsTable[15].destAddressMode.relative = 0;
	instructionsTable[15].destAddressMode.regDirect = 0;


	return instructionsTable;
}

/* returns the number of operands possible according to instruction */
int getOperandsNumber(Instruction *table, char* instruction){
	int i, operandsNumber = -1;

	for(i=0; i<NUM_OF_INSTRUCTIONS; i++){
		if(strcmp(instruction, table[i].name) == 0)
			operandsNumber = table[i].operandsNumber;
	}

	return operandsNumber;
}

/* returns the index of the instruction according to his name */
int getInstruction(Instruction *table, char *name){
	int i=0, instructionIndex = -1;

	for(i=0; i<NUM_OF_INSTRUCTIONS; i++){
		if(strcmp(name, table[i].name) == 0){
			instructionIndex = i;
			break;
		}
	}

	return instructionIndex;
}

/* checks if addressing mode if compatible with the instruction */
int checkAddressingModeCompatibility(int mode, Instruction *table, int instructionNumber, int opNumber){
	switch(mode){
		case IMMEDIATE:
			if(opNumber == SRC)
				if(table[instructionNumber].srcAddressMode.immediate == 1)
					return SUCCESS;

			if(opNumber == DEST)
				if(table[instructionNumber].destAddressMode.immediate == 1)
					return SUCCESS;
			break;


		case DIRECT:
			if(opNumber == SRC)
				if(table[instructionNumber].srcAddressMode.direct == 1)
					return SUCCESS;

			if(opNumber == DEST)
				if(table[instructionNumber].destAddressMode.direct == 1)
					return SUCCESS;
			break;


		case RELATIVE:
			if(opNumber == SRC)
				if(table[instructionNumber].srcAddressMode.relative == 1)
					return SUCCESS;

			if(opNumber == DEST)
				if(table[instructionNumber].destAddressMode.relative == 1)
					return SUCCESS;
			break;


		case REGISTER_DIRECT:
			if(opNumber == SRC)
				if(table[instructionNumber].srcAddressMode.regDirect == 1)
					return SUCCESS;

			if(opNumber == DEST)
				if(table[instructionNumber].destAddressMode.regDirect == 1)
					return SUCCESS;
			break;
	}

	return FAIL;
}