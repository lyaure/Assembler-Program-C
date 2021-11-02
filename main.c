#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "firstRun.h"
#include "secondRun.h"
#include "fileWriter.h"

#define MEM_SIZE 4096


int main(int argc, char* argv[]){
	int *IC = NULL, *DC = NULL, ic = 0, dc = 0, i;

	InstructionCode *instructionCodeTable = NULL;
	InstructionCode *dataTable = NULL;
	Symbol *symbolsTable = NULL;

	FILE *fp = NULL;
	char *fileName = NULL;

	int ret;

	/* checks for missing files */
	if(argc < 2){
		printf("ERROR, missing assembly files to preprocess\n");
		exit(0);
	}


	for(i=1; i<argc; i++){
		/* allocates extra memory for extension */ 
		fileName = (char*)malloc((strlen(argv[i])+4)*sizeof(char));

		if(fileName == NULL){
			printf("ERROR, cannot allocate memory\n");
			exit(1);
		}

		strcpy(fileName, argv[i]);
		strcat(fileName, ".as");


		fp = fopen(fileName, "r");

		/* checks for unsucessfull open operation */
		if(fp == NULL){
			printf("ERROR, cannot open file \"%s\"\n", fileName);

			free(fileName);
			exit(1);

		}

		IC = &ic;
		DC = &dc;

		ret = firstRun(fp, &instructionCodeTable, IC, &symbolsTable, &dataTable, DC);

		if(ret == SUCCESS){
			rewind(fp);

			ret = secondRun(fp, &instructionCodeTable, IC, &symbolsTable);

			if(ret == SUCCESS){
				writeFiles(argv[i], instructionCodeTable, symbolsTable, *IC-100, *DC);
			}
			else
				printf("\nERRORS IN FILE \"%s\"\n", fileName);
		}
		else
			printf("\nERRORS IN FILE \"%s\"\n", fileName);


		fclose(fp);
		freeCodeTable(&instructionCodeTable);
		freeCodeTable(&dataTable);
		freeSymbolTable(&symbolsTable);
		free(fileName);
	}

	return 0;
}