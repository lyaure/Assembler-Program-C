#ifndef SYMBOLS_LIST_H
#define SYMBOLS_LIST_H


struct symbol
{
	char *name;
	int value;
	int lineNumber;

	struct attributes
	{
		unsigned int code     : 1;
		unsigned int data 	  : 1;
		unsigned int entry	  : 1;
		unsigned int external : 1;
	}attributes;

	struct symbol *next;
};

typedef struct symbol Symbol;

void insertSymbol(Symbol **head, char *name, int value, int codeAttr, int dataAttr, int entryAttr, int externalAttr);

int isInTable(Symbol *head, char *name, int checkExternal);

int updateEntryAttribute(Symbol *head, char* name);

int getValue(Symbol *head, char *name);

int isExternal(Symbol *head, char *name);

void freeSymbolTable(Symbol **head);

void updateDataValues(Symbol **head, int IC);


#endif