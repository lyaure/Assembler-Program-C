#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "symbolsList.h"

/* insert new symbol */
void insertSymbol(Symbol **head, char *name, int value, int codeAttr, int dataAttr, int entryAttr, int externalAttr){
	Symbol *newSymbol = NULL, *current = NULL;

	newSymbol = (Symbol*)malloc(sizeof(Symbol));

	newSymbol->name = (char*)malloc((strlen(name)+1)*sizeof(char));
	strcpy(newSymbol->name, name);
	newSymbol->value = value;
	newSymbol->attributes.code = codeAttr;
	newSymbol->attributes.data = dataAttr;
	newSymbol->attributes.entry = entryAttr;
	newSymbol->attributes.external = externalAttr;

	newSymbol->next = NULL;

	if(*head == NULL)
		*head = newSymbol;
	else{
		current = *head;

		while(current->next != NULL){
			current = current->next;
		}

		current->next = newSymbol;
	}
}

/* checks if symbol is alredy in table */
int isInTable(Symbol *head, char *name, int checkExternal){
	Symbol *current = head;

	while(current != NULL){
		if(strcmp(current->name, name) == 0){
			/* label alredy in table with external attribute */
			if(checkExternal == 1){
				if(current->attributes.external == 1)
					return SUCCESS;
			}
			else
				return FAIL;
		}
		current = current->next;
	}

	return SUCCESS;
}


/* adds entry attribute to symbol */
int updateEntryAttribute(Symbol *head, char* name){
	Symbol *current = head;

	while(current != NULL){
		if(strcmp(current->name, name) == 0){
			/* label is in table, update entry attribute */
			current->attributes.entry = 1;
			return SUCCESS;
		}
		current = current->next;
	}

	return FAIL;
}

/* returns value of symbol according to his name */
int getValue(Symbol *head, char *name){
	int value = -1;

	Symbol *current = head;

	while(current != NULL){
		if(strcmp(current->name, name) == 0){
			/* label is in table */
			value = current->value;
			break;
		}
		current = current->next;
	}

	return value;
}

/* checks if symbol is external */
int isExternal(Symbol *head, char *name){
	Symbol *current = head;

	while(current != NULL){
		if(current->attributes.external == 1){
			printf("%s\n", name);
			/* label is in table */
			return SUCCESS;
		}
		current = current->next;
	}

	return FAIL;
}

/* deletes and frees symbols table */
void freeSymbolTable(Symbol **head){
	Symbol *current = *head, *next = NULL;

	while(current != NULL){
		next = current->next;

		free(current->name);
		free(current);

		current = next;
	}

	*head = NULL;
}

/* updates data addresses values acoording to IC final value */
void updateDataValues(Symbol **head, int IC){
	Symbol *current = *head;

	while(current != NULL){
		if(current->attributes.data == 1)
			current->value += IC;

		current = current->next;
	}
}