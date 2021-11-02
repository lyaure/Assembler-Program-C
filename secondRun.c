#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "secondRun.h"



int secondRun(FILE *file, InstructionCode **instructions, int *IC, Symbol **symbols){
	int ret, lineNumber = 1, errorFlag = 0;
	char *buffer = NULL;

	InstructionCode *current;
	
	buffer = (char*)malloc(BUFFER_SIZE*sizeof(char));

	if(buffer == NULL){
		printf("ERROR, cannot allocate memory\n");
		exit(1);
	}

	/* gets single line until end of file (buffer returns NULL) */
	while((fgets(buffer, BUFFER_SIZE, file)) != NULL){
		ret = secondParse(buffer, lineNumber, instructions, IC, symbols);

		if(ret == 1){
			errorFlag = 1;
		}

		lineNumber++;
	}


	current = *instructions;

	/* complete info words that have to be completed */
	while(current != NULL){
		if(current->info != NULL){
			parseInfoWord(instructions, *symbols, current->index, current->info);
		}

		current = current->next;
	}

	free(buffer);

	if(errorFlag == 1)
		return FAIL;
	else
		return SUCCESS;
}