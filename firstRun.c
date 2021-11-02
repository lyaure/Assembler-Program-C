#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "firstRun.h"



int firstRun(FILE *file, InstructionCode **instructions, int *IC, Symbol **symbols, InstructionCode **data, int *DC){
	/*initializes data counter and instruction counter*/
	int ret, errorFlag = 0, lineNumber = 1, continueToInsert = 0;
	char *buffer = NULL;

	*IC = 100;
	
	buffer = (char*)malloc(BUFFER_SIZE*sizeof(char));

	if(buffer == NULL){
		printf("ERROR, cannot allocate memory\n");
		exit(1);
	}

	/* gets single line until end of file (buffer returns NULL) */
	while((fgets(buffer, BUFFER_SIZE, file)) != NULL){
		ret = parseStr(buffer, lineNumber, instructions, IC, symbols, data, DC, continueToInsert);

		if(ret == 1){
			errorFlag = 1;
			continueToInsert = 1;
		}

		lineNumber++;
	}


	updateDataValues(symbols, *IC);

	join(instructions, data);

	free(buffer);	

	if(errorFlag == 1)
		return FAIL;

	return SUCCESS;
}