#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileWriter.h"

/* creates output files according to instruction code and symbols table */
void writeFiles(char *filename, InstructionCode *instructions, Symbol *symbols, int IC, int DC){
	char *objectFileName = NULL, *entriesFileName = NULL, *externalsFileName = NULL;
	int entries = 0, externals = 0;
	FILE *objectFile = NULL, *entriesFile = NULL, *externalsFile = NULL;
	InstructionCode *currentInstruction = instructions;
	Symbol *currentSymbol = symbols;

	objectFileName = (char*)malloc((strlen(filename)+4)*sizeof(char));

	if(objectFileName == NULL){
		printf("ERROR, cannot allocate memory\n");
		exit(1);
	}

	strcpy(objectFileName, filename);
	strcat(objectFileName, ".ob");


	objectFile = fopen(objectFileName, "w");

	if(objectFile == NULL){
		printf("ERROR, cannot create and open file '%s'.\n", objectFileName);
		free(objectFileName);
		exit(1);
	}

	fprintf(objectFile, "%d %d\n", IC, DC);

	while(currentInstruction != NULL){
		fprintf(objectFile, "%04d %03X %c\n", currentInstruction->index, *(int*)&currentInstruction->word, currentInstruction->attribute);

		/* external symbol in instruction */
		if(currentInstruction->attribute == 'E'){
			/* externals file not created yet */
			if(externals == 0){
				externalsFileName = (char*)malloc((strlen(filename)+5)*sizeof(char));
				
				if(externalsFileName == NULL){
					printf("ERROR, cannot allocate memory\n");
					exit(1);
				}

				strcpy(externalsFileName, filename);
				strcat(externalsFileName, ".ext");

				externalsFile = fopen(externalsFileName, "w");

				if(externalsFile == NULL){
					printf("ERROR, cannot create and open file '%s'.\n", externalsFileName);
					free(objectFileName);
					free(externalsFileName);
					fclose(objectFile);
					exit(1);
				}
				externals = 1;
			}

			fprintf(externalsFile, "%s %d\n", currentInstruction->info, currentInstruction->index);
		}

		currentInstruction = currentInstruction->next;
	}

	/* checks for entries and create output file if needed */
	while(currentSymbol != NULL){
		if(currentSymbol->attributes.entry == 1){
			if(entries == 0){
				entriesFileName = (char*)malloc((strlen(filename)+5)*sizeof(char));
				
				if(entriesFileName == NULL){
					printf("ERROR, cannot allocate memory\n");
					exit(1);
				}

				strcpy(entriesFileName, filename);
				strcat(entriesFileName, ".ent");

				entriesFile = fopen(entriesFileName, "w");

				if(entriesFile == NULL){
					printf("ERROR, cannot create and open file '%s'.\n", entriesFileName);
					free(objectFileName);
					free(entriesFileName);
					fclose(objectFile);
					exit(1);
				}
				entries = 1;
			}

			fprintf(entriesFile, "%s %d\n", currentSymbol->name, currentSymbol->value);
		}

		currentSymbol = currentSymbol->next;
	}

	if(externals == 1){
		free(externalsFileName);
		fclose(externalsFile);
	}

	if(entries == 1){
		free(entriesFileName);
		fclose(entriesFile);
	}

	free(objectFileName);
	fclose(objectFile);
}