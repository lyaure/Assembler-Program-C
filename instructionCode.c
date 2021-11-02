#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instructionCode.h"

/* insert new word */
void insertCode(InstructionCode **head, int IC, int dest, int src, int funct, int opcode, char attribute, char *information){
	InstructionCode *newCode = NULL, *current = NULL;

	newCode = (InstructionCode*)calloc(1, sizeof(InstructionCode));

	if(newCode == NULL){
		printf("ERROR, cannot allocate memory\n");
		exit(1);
	}

	newCode->index = IC;
	newCode->word.dest = dest;
	newCode->word.src = src;
	newCode->word.funct = funct;
	newCode->word.opcode = opcode;
	newCode->attribute = attribute;

	if(information != NULL){
		newCode->info = (char*)malloc((strlen(information)+1)*sizeof(char));

		if(newCode->info == NULL){
			printf("ERROR, cannot allocate memory\n");
			exit(1);
		}
		strcpy(newCode->info, information);
	}
	else
		newCode->info = NULL;


	newCode->next = NULL;


	if(*head == NULL)
		*head = newCode;
	else{
		current = *head;

		while(current->next != NULL){
			current = current->next;
		}

		current->next = newCode;
	}
}

/* update incomplete word */ 
void completeWord(InstructionCode **head, int index, int dest, int src, int funct, int opcode, char attribute){
	InstructionCode *current = *head;

	while(current != NULL){
		if(current->index == index){
			current->word.dest = dest;
			current->word.src = src;
			current->word.funct = funct;
			current->word.opcode = opcode;
			current->attribute = attribute;

			if(attribute == 'E')
				if(current->info[0] == '%'){
					memmove(current->info, current->info + 1, strlen(current->info + 1));
				}

		}

		current = current->next;
	}
}

/* join instruction table and data table to one table and update data indexes */
void join(InstructionCode **head1, InstructionCode **head2){
	int lastIndex = 0;
	InstructionCode *current = *head1;

	if(*head1 == NULL || *head2 == NULL)
		return;

	while(current->next != NULL)
		current = current->next;


	lastIndex = current->index;
	lastIndex++;

	current->next = *head2;

	current = current->next;

	while(current != NULL){
		current->index += lastIndex;
		current = current->next;
	}

	*head2 = NULL;

}

/* deletes and frees instruction code table */
void freeCodeTable(InstructionCode **head){
	InstructionCode *current = *head, *next = NULL;

	if(*head == NULL)
		return;

	while(current != NULL){
		next = current->next;

		if(current->info != NULL){
			free(current->info);
		}

		free(current);

		current = next;
	}

	*head = NULL;
}