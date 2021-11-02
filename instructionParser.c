#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "globals.h"
#include "instructionParser.h"

#define A 65
#define Z 90
#define a 97
#define z 122
#define ZERO 48
#define NINE 57


char *reservedWords[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
						"dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop",
						"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
						"data", "string", "entry", "extern"};

char *registerNames[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};


/* parse of the first run skips empty or comments lines*/
int parseStr(const char *line, const int lineNumber, InstructionCode **instructionCode, int *IC, Symbol **symbols, InstructionCode **dataTable, int *DC, int continueToInsert){
	int ret;

	/* comment line */
	if(line[0] == ';'){
		return SUCCESS;
	}

	/* empty line */
	if(isStrEmpty(line) == SUCCESS){
		return SUCCESS;
	}

	ret = parseLine(line, lineNumber, instructionCode, IC, symbols, dataTable, DC, continueToInsert);

	if(ret == 1)
		return FAIL;
	else
		return SUCCESS;
}

/* parse of the second run skips empty or comments lines*/
int secondParse(const char *line, const int lineNumber, InstructionCode **instructionCode, int *IC, Symbol **symbols){
	int ret;

	/* comment line */
	if(line[0] == ';'){
		return SUCCESS;
	}

	/* empty line */
	if(isStrEmpty(line) == SUCCESS){
		return SUCCESS;
	}


	ret = searchForEntryDirective(line, lineNumber, instructionCode, IC, symbols);

	if(ret == 1)
		return FAIL;
	else
		return SUCCESS;

}

/*checks if string contains only white sapces or/and tabs */ 
int isStrEmpty(const char *str){
	int i, isEmpty = SUCCESS;

	for(i = 0; i<strlen(str); i++){
		if(str[i] != ' ' && str[i] != '\n' && str[i] != '\t' && str[i] != '\r'){
			isEmpty = FAIL;
			break;
		}
	}

	return isEmpty;
}

/* parse line to be able to define witch type of statement it is  and redirect it to parsing according to type*/
int parseLine(const char *str, const int lineNumber, InstructionCode **instructionCode, int *IC, Symbol **symbols, InstructionCode **dataTable, int *DC, int continueToInsert){
	char *instruction = NULL, *token = NULL, *label = NULL, *copyInstruction = NULL;
	int labelFlag = 0, ret = 0, errorFlag = 0, directive  = -1;
	int codeAttr = 0, dataAttr = 0, entryAttr = 0, externalAttr = 0;

	removePaddingSpaces(str, &instruction);

	copyInstruction = (char*)malloc((strlen(instruction)+1)*sizeof(char));

	if(copyInstruction == NULL){
		printf("ERROR, cannot allocate memory\n");
		exit(1);
	}

	strcpy(copyInstruction, instruction);

	token = strtok(instruction, ":");

	/* first token is label */
	if(strcmp(token, copyInstruction) != 0){
		labelFlag = 1;

		label = (char*)malloc((strlen(token)+1)*sizeof(char));

		if(label == NULL){
			printf("ERROR, cannot allocate memory\n");
			exit(1);
		}

		strcpy(label, token);

		token = strtok(NULL, " \t");
	}
	else{
		strcpy(instruction, copyInstruction);
		token = strtok(instruction, " \t");
	}


	if(strcmp(token, ".data") == 0)
		directive = DATA;

	if(strcmp(token, ".string") == 0)
		directive = STRING;

	/* checks for data or string directive */
	if(directive == DATA || directive == STRING){
		if(labelFlag == 1){
			dataAttr = 1;

			ret = isLabelLegal(label, lineNumber);

			/* label name is not legal */
			if(ret == FAIL){
				errorFlag = 1;
			}
			else{
				ret = isInTable(*symbols, label, 0);

				/* label already exists */
				if(ret == FAIL){
					errorFlag = 1;
					printf("ERROR (line %d): Label name already exists\n", lineNumber);
				}
				else{
					if(continueToInsert == 0)
						insertSymbol(symbols, label, *DC, codeAttr, dataAttr, entryAttr, externalAttr);
				}
			}
		}

		switch(directive){
		case DATA:
			/* gets data from the directive */
			token = strtok(NULL, "\0");
						
			ret = parseDataDirective(token, dataTable, DC, lineNumber, continueToInsert);

			if(ret == 1)
				errorFlag = 1;

			free(instruction);
			free(copyInstruction);

			if(label)
				free(label);

			if(errorFlag == 1)
				return FAIL;
			else
				return SUCCESS;
		

		case STRING:
			/* gets data from the directive */
			token = strtok(NULL, "\0");
						
			ret = parseStringDirective(token, dataTable, DC, lineNumber, continueToInsert);

			if(ret == 1)
				errorFlag = 1;

			free(instruction);
			free(copyInstruction);

			if(label)
				free(label);

			if(errorFlag == 1)
				return FAIL;
			else
				return SUCCESS;
		}
	}

	if(strcmp(token, ".extern") == 0)
		directive = EXTERN;

	if(strcmp(token, ".entry") == 0)
		directive = ENTRY;

	if(directive == EXTERN || directive == ENTRY){
		if(directive == ENTRY){
			free(instruction);
			free(copyInstruction);
			
			if(label)
				free(label);

			if(errorFlag == 1)
				return FAIL;
			else
				return SUCCESS;
		}
		/* extern directive */
		else{
			externalAttr = 1;

			if(labelFlag == 1)
				printf("WARNING (line %d): Unnecessary definition of label before extern directive\n", lineNumber);

			if(label)
				free(label);

			label = strtok(NULL, " \t");

			if(label == NULL){
				printf("ERROR (line %d): Missing label after extern directive.\n", lineNumber);
				free(instruction);
				free(copyInstruction);
				return FAIL;
			}
			else{
				ret = isLabelLegal(label, lineNumber);

				/* label name is not legal */
				if(ret == FAIL){
					errorFlag = 1;
					printf("ERROR (line %d): Label name is not legal\n", lineNumber);
				}
				else{
					ret = isInTable(*symbols, label, 1);

					/* label already exists */
					if(ret == FAIL){
						errorFlag = 1;
						printf("ERROR (line %d): Label name already exists\n", lineNumber);
					}
					else{
						token = strtok(NULL, " \t");

						if(token == NULL){
							if(continueToInsert == 0 && errorFlag == 0)
								insertSymbol(symbols, label, 0, codeAttr, dataAttr, entryAttr, externalAttr);

							free(instruction);
							free(copyInstruction);

							return SUCCESS;
						}
						/* forbidden data after label */
						else{
							printf("ERROR (line %d): Not expecting data after label name\n", lineNumber);

							free(instruction);
							free(copyInstruction);

							return FAIL;
						}
					}
				}
			}
		}
	}

	free(instruction);

	/* instruction line */
	codeAttr = 1;

	if(labelFlag == 1){
		ret = isLabelLegal(label, lineNumber);

		/* label name is not legal */
		if(ret == FAIL){
			errorFlag = 1;
		}
		else{
			ret = isInTable(*symbols, label, 0);

			/* label already exists */
			if(ret == FAIL){
				errorFlag = 1;
				printf("ERROR (line %d): Label name already exists\n", lineNumber);

			}
			else{
				if(errorFlag == 0 && continueToInsert == 0){
					insertSymbol(symbols, label, *IC, codeAttr, dataAttr, entryAttr, externalAttr);
				}
			}
		}

		/* prepares instruction without label */
		token = strtok(copyInstruction, ":");
		token = strtok(NULL, "\0");
	}
	else
		token = copyInstruction;


	parseInstruction(token, instructionCode, IC, lineNumber, continueToInsert);


	free(copyInstruction);
	
	if(label)
		free(label);


	return SUCCESS;
}

/* removes white spaces and tabs from start and end of the string */
void removePaddingSpaces(const char *str, char **newStr){
	int i = 0;
	char *tmpStr = NULL;
	char *strCopy = (char*)malloc((strlen(str)+1)*sizeof(char));

	if(strCopy == NULL){
		printf("ERROR, cannot allocate memory\n");
		exit(1);
	}

	strcpy(strCopy, str);

	tmpStr = strCopy;

	/* removes white spaces and tabs from start of string */
	for(i = 0; str[i] == ' ' || str[i] == '\t'; i++)
		tmpStr++;

	/* removes white spaces, tabs and new line from start of string */
	while(tmpStr[strlen(tmpStr)-1] == ' ' || tmpStr[strlen(tmpStr)-1] == '\t' || tmpStr[strlen(tmpStr)-1] == '\n' || tmpStr[strlen(tmpStr)-1] == '\r')
		tmpStr[strlen(tmpStr)-1] = '\0';

	*newStr = (char*)malloc((strlen(tmpStr)+1)*sizeof(char));

	 if(newStr == NULL){
	 	printf("ERROR, cannot allocate memory\n");
	 	free(strCopy);
		exit(1);
	 }

	strcpy(*newStr, tmpStr);

	free(strCopy);
}

/* checks if label is legal */
int isLabelLegal(char *label, const int lineNumber){
	int i = 0, errorFlag = 0, numOfReservedWords;

	if(strlen(label) == 0){
		printf("ERROR (line %d): Label name cannot be empty.\n", lineNumber);
		return FAIL;
	}

	/* checks if label starts with letter */
	if(isalpha(label[0]) == 0){
		printf("ERROR (line %d): Label name must start with letter.\n", lineNumber);
		errorFlag = 1;
	}

	/* checks if label conatins only letters or digits */
	for(i=0; i<strlen(label); i++){
		if((isalpha(!label[i]) && !isdigit(label[i])) || label[i] == ' ' || label[i] == '\t'){
			printf("ERROR (line %d): Label name must contain only letters and digits and cannot contain spaces.\n", lineNumber);
			errorFlag = 1;
			break;
		}
	}

	numOfReservedWords = sizeof(reservedWords)/sizeof(char*);

	/* checks if label is a reserved word */
	for(i=0; i<numOfReservedWords; i++){
		if(strcmp(label, reservedWords[i]) == 0){
			printf("ERROR (line %d): Label name cannot be a reserved word.\n", lineNumber);
			errorFlag = 1;
		}
	}


	if(strlen(label) > 31){
		printf("ERROR (line %d): Label length must be under 31 characters.\n", lineNumber);
		errorFlag = 1;
	}

	if(errorFlag == 1){
		return FAIL;
	}

	return SUCCESS;
}

/* parses the data directive and fills the data table */
int parseDataDirective(char *data, InstructionCode **dataTable, int *DC, const int lineNumber, int continueToInsert){
	int i, dataNum, isNegative = 0, errorFlag = 0, status = START, dest = 0, src = 0, funct = 0, opcode = 0, numOfCommas = 0, forbiddenChars = 0;
	char c, next, *newData = NULL, attribute = 'A';

	if(data == NULL){
		printf("ERROR (line %d): Expecting data after data directive.\n", lineNumber);
		return FAIL;
	}

	removePaddingSpaces(data, &newData);

	if(newData == NULL){
		printf("ERROR (line %d): Expecting data after data directive.\n", lineNumber);
		return FAIL;
	}

	for(i=0; i<strlen(newData); i++){
		c = newData[i];

		switch(status){
			case START:
				isNegative = 0;

				if(numOfCommas > 1){
					printf("ERROR (line %d): Too many commas ',' in statement.\n", lineNumber);
					errorFlag = 1;
					numOfCommas = -1;
				}
				else
					numOfCommas = 0;

				if(c == '-' || c == '+'){
					if(c == '-')
						isNegative = 1;
					status = OPERATOR;
				}
				else if(isdigit(c)){
					dataNum = c - '0';
					status = DIGIT;
				}
				else if(c == ','){
					if(numOfCommas != -1)
						numOfCommas++;

					if(forbiddenChars == 0)
						printf("ERROR (line %d): Forbidden comma ',' at start.\n", lineNumber);
					
					errorFlag = 1;
				}
				else{
					if(forbiddenChars == 0)
						printf("ERROR (line %d): Forbidden characters in data, data must contain only digits.\n", lineNumber);
					forbiddenChars = 1;
					errorFlag = 1;
				}
				break;

			case OPERATOR:
				if(isdigit(c)){
					dataNum = c - '0';
					status = DIGIT;
				}
				else if(c == ','){
					printf("ERROR (line %d): Missing number before comma ',' .\n", lineNumber);
					errorFlag = 1;

					status = COMMA;
				}
				else if(c == ' ' || c == '\t'){
					printf("ERROR (line %d): No spaces allowed after operator.\n", lineNumber);
					errorFlag = 1;

					next = newData[i+1];

					while(next == '-' || next == '+'){
						i++;
						next = newData[i+1];
					}
				}
				else if(c == '-' || c == '+'){
					printf("ERROR (line %d): Too many operators before number.\n", lineNumber);
					errorFlag = 1;
				}
				else{
					if(forbiddenChars == 0)
						printf("ERROR (line %d): Forbidden characters in data, data must contain only digits.\n", lineNumber);
					forbiddenChars = 1;
					errorFlag = 1;
				}

				break;
			
			case DIGIT:
				if(isdigit(c)){
					dataNum *= 10;
					dataNum += c - '0';
					status = DIGIT;
				}
				else if(c == ','){
					if(numOfCommas != -1)
						numOfCommas++;

					if(isNegative == 1)
						dataNum *= -1;

					if(errorFlag != 1){
						prepareWord(dataNum, &dest,	&src, &funct, &opcode);
						insertCode(dataTable, *DC, dest, src, funct, opcode, attribute, NULL);
						*DC += 1;
					}

					status = COMMA;
				}
				else if(c == ' ' || c == '\t'){
					next = newData[i+1];

					while(next == ' ' || next == '\t'){
						i++;
						next = newData[i+1];
					}

					if(isdigit(next)){
						printf("ERROR (line %d): No spaces allowed between digits.\n", lineNumber);
						errorFlag = 1;
					}
					else if(next != ','){
						if(forbiddenChars == 0)
						printf("ERROR (line %d): Forbidden characters in data, data must contain only digits.\n", lineNumber);
					forbiddenChars = 1;
					errorFlag = 1;
					}
				}
				else{
					if(forbiddenChars == 0)
						printf("ERROR (line %d): Forbidden characters in data, data must contain only digits.\n", lineNumber);
					forbiddenChars = 1;
					errorFlag = 1;
				}

				break;

			case COMMA:
				if(c == ','){
					if(numOfCommas != -1)
						numOfCommas++;

					next = newData[i+1];

					while(next == ' ' || next == '\t' || next == ','){
						i++;
						next = newData[i];
					}

					status = COMMA;
				}
				else if(c == ' ' || c == '\t'){
					next = newData[i+1];

					while(next == ' ' || next == '\t'){
						i++;
						next = newData[i+1];
					}
				}
				else{
					i--;
					status = START;
				}
				
				break;
		}
	}


	if(numOfCommas > 1){
		printf("ERROR (line %d): Too many commas ',' in statement.\n", lineNumber);
		errorFlag = 1;
	}

	if(newData[strlen(newData)-1] == ','){
		printf("ERROR (line %d): Redundant comma at end of statement.\n", lineNumber);
		errorFlag = 1;
	}

	if(errorFlag == 0){
		if(isNegative == 1)
		dataNum *= -1;

		prepareWord(dataNum, &dest,	&src, &funct, &opcode);
		insertCode(dataTable, *DC, dest, src, funct, opcode, attribute, NULL);
		*DC += 1;
		
		free(newData);
		return SUCCESS;
	}
	else{
		free(newData);
		return FAIL;
	}
}

/* parses the string directive and fills the data table */
int parseStringDirective(char *data, InstructionCode **dataTable, int *DC, const int lineNumber, int continueToInsert){
	int i=0, dest = 0, src = 0, funct = 0, opcode = 0, asciiCode = 0;
	char *newData = NULL, attribute = 'A';

	if(data == NULL){
		printf("ERROR (line %d): Expecting data after string directive.\n", lineNumber);
		return FAIL;
	}

	removePaddingSpaces(data, &newData);

	if(newData == NULL){
		printf("ERROR (line %d): Expecting data after string directive.\n", lineNumber);
		free(newData);
		return FAIL;
	}

	/* checks if the string is legal */
	if(newData[i] != '"' || newData[strlen(newData)-1] != '"'){
		printf("ERROR (line %d): Legal quote marks are missing, string is not valid.\n", lineNumber);
		
		free(newData);
		return FAIL;
	}

	for(i=1; i<strlen(newData)-1; i++){
		asciiCode = (int)newData[i];
		prepareWord(asciiCode, &dest,	&src, &funct, &opcode);
		insertCode(dataTable, *DC, dest, src, funct, opcode, attribute, NULL);
		*DC += 1;
	}

	insertCode(dataTable, *DC, 0, 0, 0, 0, attribute, NULL);
	*DC += 1;

	free(newData);

	return SUCCESS;
}

/* parse instruction according to number of operands and addressing modes */
int parseInstruction(char *instruction, InstructionCode **instructionCode, int *IC, const int lineNumber, int continueToInsert){
	char *newInstruction = NULL, *token = NULL, attribute = ' ', *operand1 = NULL, *operand2 = NULL;
	int instructionNumber, operandsNumber, errorFlag = 0, dest, src, funct, opcode, mode, ret;
	
	Instruction *instructionTable = NULL;
	instructionTable = initInstructionsTable();

	removePaddingSpaces(instruction, &newInstruction);

	token = strtok(newInstruction, " \t");

	if(token[strlen(token)-1] == ','){
		printf("ERROR (line %d): Commas ',' are not allowed after instruction name.\n", lineNumber);
		errorFlag = 1;

		while(token[strlen(token)-1] == ',')
			token[strlen(token)-1] = '\0';
	}

	instructionNumber = getInstruction(instructionTable, token);

	if(instructionNumber < 0){
		printf("ERROR (line %d): No such instruction as '%s'.\n", lineNumber, token);
		free(newInstruction);
		return FAIL;
	}


	operandsNumber = instructionTable[instructionNumber].operandsNumber;

	switch(operandsNumber){
		case NO_OPERANDS:
			if(continueToInsert == 0 && errorFlag == 0){
				dest = 0;
				src = 0;
				funct = instructionTable[instructionNumber].funct;
				opcode = instructionTable[instructionNumber].opcode;

				attribute = 'A';

				insertCode(instructionCode, *IC, dest, src, funct, opcode, attribute, NULL);
				*IC += 1;	
			}

			token = strtok(NULL, "\0");

			if(token != NULL){
				printf("ERROR (line %d): Not expecting operands after instruction.\n", lineNumber);
				free(newInstruction);
				return FAIL;
			}

			break;

		
		case ONE_OPERANDS:	
			operand1 = strtok(NULL, " \t");

			if(operand1 != NULL && operand1[0] == ','){
				printf("ERROR (line %d): Commas ',' are not allowed after instruction name.\n", lineNumber);
				errorFlag = 1;

				operand1 = strtok(operand1, " ,");
			}

			if(operand1 == NULL){
				printf("ERROR (line %d): Expecting operand after instruction.\n", lineNumber);
				errorFlag = 1;
				break;
			}

			mode = getAddressingMode(operand1);

			ret = checkAddressingModeCompatibility(mode, instructionTable, instructionNumber, DEST);

			if(ret == 1){
				errorFlag = 1;
				printf("ERROR (line %d): Instruction '%s' is not compatible with this addressing mode.\n", lineNumber, instructionTable[instructionNumber].name);
			}


			if(continueToInsert == 0 && errorFlag == 0){

				dest = mode;
				src = 0;
				funct = instructionTable[instructionNumber].funct;
				opcode = instructionTable[instructionNumber].opcode;

				attribute = 'A';

				insertCode(instructionCode, *IC, dest, src, funct, opcode, attribute, NULL);
				*IC += 1;

				ret = insertInfoWord(instructionCode, instructionTable, instructionNumber, operand1, IC, continueToInsert, lineNumber);

				if(ret == 1)
					errorFlag = 1;
			}

			token = strtok(NULL, "\0");

			if(token != NULL){
				printf("ERROR (line %d): Not expecting more than one operand after instruction.\n", lineNumber);
				errorFlag = 1;
			}

			break;

		
		case TWO_OPERANDS:
			token = strtok(NULL, "\0");

			if(token != NULL){
				ret = parseOperands(token, &operand1, &operand2, lineNumber);

				if(ret == 1)
					errorFlag = 1;
			}

			if(operand1 == NULL && operand2 != NULL){
				printf("ERROR (line %d): Expecting operand after instruction.\n", lineNumber);
				errorFlag = 1;
				break;
			}

			if(operand2 == NULL){
				printf("ERROR (line %d): Expecting two operands after instruction.\n", lineNumber);
				errorFlag = 1;
				free(operand1);
				break;
			}

			src = getAddressingMode(operand1);
			dest = getAddressingMode(operand2);

			if(checkAddressingModeCompatibility(src, instructionTable, instructionNumber, SRC) == 1 ||
				checkAddressingModeCompatibility(dest, instructionTable, instructionNumber, DEST) == 1){
				errorFlag = 1;
				printf("ERROR (line %d): Instruction '%s' is not compatible with this addressing mode.\n", lineNumber, instructionTable[instructionNumber].name);
			}


			if(continueToInsert == 0 && errorFlag == 0){
				funct = instructionTable[instructionNumber].funct;
					opcode = instructionTable[instructionNumber].opcode;

					attribute = 'A';

					insertCode(instructionCode, *IC, dest, src, funct, opcode, attribute, NULL);
					*IC += 1;

					ret = insertInfoWord(instructionCode, instructionTable, instructionNumber, operand1, IC, continueToInsert, lineNumber);

					if(ret == 1)
						errorFlag = 1;

					ret = insertInfoWord(instructionCode, instructionTable, instructionNumber, operand2, IC, continueToInsert, lineNumber);

					if(ret == 1)
						errorFlag = 1;

					free(operand1);
					free(operand2);
			}

			break;
	}

	free(newInstruction);


	if(errorFlag == 1)
		return FAIL;
	else
		return SUCCESS;
}

/* checks if string is a register name */
int checkIfRegister(char *str){
	int i;

	for(i=0; i<NUM_OF_REGISTERS; i++){
		if(strcmp(str, registerNames[i]) == 0)
			return SUCCESS;
	}

	return FAIL;
}

/* returns the addressing mode according to operand */
int getAddressingMode(char *operand){
	/* immediate mode */
	if(operand[0] == '#'){
		return IMMEDIATE;
	}
	/* relative mode */
	else if(operand[0] == '%'){
		return RELATIVE;
	}
	/* register direct mode */
	else if(checkIfRegister(operand) == SUCCESS){
		return REGISTER_DIRECT;
	}
	/* direct mode */
	else{
		return DIRECT;
	}
}

/* inserts info word to instruction table with information or not to be able to complete instruction code in second run */
int insertInfoWord(InstructionCode **instructionCode, Instruction *instructionTable, int instructionNumber, char *operand, int *IC, int continueToInsert, int lineNumber){
	int ret = 0, errorFlag = 0, mode = -1;

	mode = getAddressingMode(operand);

	switch(mode){
		case IMMEDIATE:
			ret = immediateMode(instructionCode, instructionTable, instructionNumber, operand, IC, continueToInsert, lineNumber);

			if(ret == FAIL)
				errorFlag = 1;
					
			break;

		case DIRECT:
			ret = directMode(instructionCode, instructionTable, instructionNumber, operand, IC, continueToInsert, lineNumber);

			if(ret == FAIL)
				errorFlag = 1;

			break;

		case RELATIVE:
			ret = relativeMode(instructionCode, instructionTable, instructionNumber, operand, IC, continueToInsert, lineNumber);

			if(ret == FAIL)
				errorFlag = 1;

			break;

		case REGISTER_DIRECT:
			ret = registerDirectMode(instructionCode, instructionTable, instructionNumber, operand, IC, continueToInsert, lineNumber);

			if(ret == FAIL)
				errorFlag = 1;
			break;
	}

	if(errorFlag == 1)
		return FAIL;
	else
		return SUCCESS;
}

/* insert word according to immediate mode without information */
int immediateMode(InstructionCode **instructionCode, Instruction *instructionTable, int instructionNumber, char *operand, int *IC, int continueToInsert, int lineNumber){
	int dest, src, funct, opcode, ret, num = 0;
	char attribute = 'A';

	ret = checkImmediateOp(operand, &num, lineNumber);

	if(ret == FAIL)
		return FAIL;
	

	prepareWord(num, &dest, &src, &funct, &opcode);

	insertCode(instructionCode, *IC, dest, src, funct, opcode, attribute, NULL);
	*IC += 1;

	return SUCCESS;
}

/* insert word according to direct mode with information */
int directMode(InstructionCode **instructionCode, Instruction *instructionTable, int instructionNumber, char *operand, int *IC, int continueToInsert, int lineNumber){
	char *information = NULL;

	int size = (int)(ceil(log10(lineNumber))) + strlen(operand) + 3;

	information = (char*)malloc(size*sizeof(char));

	sprintf(information, "%s %d", operand, lineNumber);

	insertCode(instructionCode, *IC, 0, 0, 0, 0, '-', information);
	*IC += 1;

	free(information);

	return SUCCESS;
}

/* insert word according to relative mode with information */
int relativeMode(InstructionCode **instructionCode, Instruction *instructionTable, int instructionNumber, char *operand, int *IC, int continueToInsert, int lineNumber){
	char *information = NULL;

	int size = (int)(ceil(log10(lineNumber))) + strlen(operand) + 3;

	information = (char*)malloc(size*sizeof(char));

	sprintf(information, "%s %d", operand, lineNumber);

	insertCode(instructionCode, *IC, 0, 0, 0, 0, '-', information);
	*IC += 1;

	free(information);

	return SUCCESS;
}

/* insert word according to register direct mode without information */
int registerDirectMode(InstructionCode **instructionCode, Instruction *instructionTable, int instructionNumber, char *operand, int *IC, int continueToInsert, int lineNumber){
	int dest, src, funct, opcode, regNumber = 0;
	char attribute = 'A';

	regNumber = operand[1] - '0';

	prepareWord(pow(2, regNumber), &dest, &src, &funct, &opcode);

	insertCode(instructionCode, *IC, dest, src, funct, opcode, attribute, NULL);
	*IC += 1;

	return SUCCESS;
} 

/* checks if immediate operand is legal */
int checkImmediateOp(char *operand, int *number, int lineNumber){
	int i = 1, isNegative = 0;

	if(operand[i] == '-'){
		isNegative = 1;
		i++;
	}
	else if(operand[i] == '+'){
		i++;
	}

	for( ; i<strlen(operand); i++){
		if(isdigit(operand[i])){
			*number *= 10;
			*number += operand[i] - '0';
		}
		else{
			printf("ERROR (line %d): Operand must be Integer.\n", lineNumber);
			return FAIL;
		}
	}

	if(isNegative == 1)
		*number *= -1;

	return SUCCESS;
}

/* prepares binary instruction code word */
void prepareWord(int num, int *dest, int *src, int *funct, int *opcode){
	unsigned int mask;

	mask = 1 << (WORD_SIZE -1);

	fillVariable(num, &mask, opcode, 4);
	fillVariable(num, &mask, funct, 4);
	fillVariable(num, &mask, src, 2);
	fillVariable(num, &mask, dest, 2);
}

/* fill variables that compose instruction word according to value */
void fillVariable(int num, unsigned int *mask, int *var, int varSize){
	int bitValue, i;

	*var = 0;

	bitValue = pow(2, varSize-1);

	for(i=0; i<varSize; i++){
		if(num&*mask)
			*var += bitValue;

		bitValue /= 2;

		*mask >>= 1;
	}
}


/* parse instructions with two operands to two operands */
int parseOperands(char *token, char **operand1, char **operand2, int lineNumber){
	int i=0, status, operandLength = 0, errorFlag = 0, startIndex=-1, numOfCommas = 0, finish = 0;
	char *operandsToParse = NULL;

	removePaddingSpaces(token, &operandsToParse);

	status = START;

	for(i=0; i<strlen(operandsToParse); i++){
		if(finish ==1)
			break;

		switch(status){
			case START:
				if(operandsToParse[i] == ','){
					printf("ERROR (line %d): Forbidden comma after instruction name.\n", lineNumber);

					while(operandsToParse[i+1] == ',' || operandsToParse[i+1] == ' ' || operandsToParse[i+1] == '\t')
						i++;
				}
				else
					i--;

				status = OPERAND1;
				break;
			
			case OPERAND1:
				if(operandsToParse[i] != ' ' && operandsToParse[i] != '\t' && operandsToParse[i] != ','){
					if(startIndex < 0)
						startIndex = i;

					operandLength++;
				}
				else{
					while(operandsToParse[i] == ' ' || operandsToParse[i] == '\t'){
						i++;
					}

					if(operandsToParse[i] == ','){	
						numOfCommas++;
						
						status = COMMA;
					}
					else{
						printf("ERROR (line %d): Expecting one comma ',' between operands.\n", lineNumber);
						errorFlag = 1;
						status = OPERAND2;
					}

					*operand1 = (char*)calloc((operandLength+2), sizeof(char));

						if(*operand1 == NULL){
							printf("ERROR, cannot allocate memory\n");
							exit(1);
						}
						
						strncpy(*operand1, operandsToParse + startIndex, operandLength);

						operandLength = 0;
						startIndex = -1;

				}
				break;

			case OPERAND2:
				if(operandsToParse[i] != ' ' && operandsToParse[i] != '\t' && operandsToParse[i] != ','){
					if(startIndex < 0)
						startIndex = i;

					operandLength ++;
				}
				else if(operandsToParse[i] == ' ' || operandsToParse[i] == '\t' || operandsToParse[i] == '\0'){
					finish = 1;
				}
				else{
					printf("ERROR (line %d): Forbidden comma ',' at end of instruction.\n", lineNumber);
					errorFlag = 1;
					finish = 1;
				}
				break;

			case COMMA:
				if(operandsToParse[i] == ','){
					printf("ERROR (line %d): Too many commas ',' between operands.\n", lineNumber);

					while(operandsToParse[i+1] == ',' || operandsToParse[i+1] == ' ' || operandsToParse[i+1] == '\t')
						i++;

					status = OPERAND2;
				}
				else if(operandsToParse[i] == ' ' || operandsToParse[i] == '\t'){
					while(operandsToParse[i+1] == ' ' || operandsToParse[i+1] == '\t')
						i++;

					status = COMMA;
				}
				else{
					i--;
					status = OPERAND2;
				}
				break;
		}
	}

	if(status == OPERAND1){
		*operand1 = (char*)calloc((operandLength+1), sizeof(char));

		if(*operand1 == NULL){
			printf("ERROR, cannot allocate memory\n");
			exit(1);
		}
							
		strncpy(*operand1, operandsToParse + startIndex, operandLength);
	}
	else if(status == OPERAND2){

		*operand2 = (char*)calloc((operandLength+1), sizeof(char));

		if(*operand2 == NULL){
			printf("ERROR, cannot allocate memory\n");
			exit(1);
		}

		strncpy(*operand2, operandsToParse + startIndex, operandLength);
	}

	if(i < strlen(operandsToParse)){
		printf("ERROR (line %d): Not expecting more than two operands\n", lineNumber);
	}

	free(operandsToParse); 


	if(errorFlag == 1)
		return FAIL;
	else
		return SUCCESS;
}

/* search for entry directives and skip the rest */
int searchForEntryDirective(const char *line, const int lineNumber, InstructionCode **instructionCode, int *IC, Symbol **symbols){
	char *token = NULL, *copyLine = NULL, *label = NULL;
	int ret = 0;

	removePaddingSpaces(line, &copyLine);

	token = strtok(copyLine, " \t");

	/* first token is label */
	if(token[strlen(token)-1] == ':'){
		token = strtok(NULL, " \t");
	}

	if(strcmp(token, ".entry") == 0){
		label = strtok(NULL, " \t");

		if(label != NULL){
			ret = updateEntryAttribute(*symbols, label);

			if(ret == FAIL){
				printf("ERROR (line %d): Label '%s' is undefined, cannot update entry attribute.\n", lineNumber, token);
				free(copyLine);
				return FAIL;
			}
			else{
				free(copyLine);
				return SUCCESS;
			}
		}
		else{
			printf("ERROR (line %d): Missing label after extern directive.\n", lineNumber);
			free(copyLine);
			return SUCCESS;
		}
	}
	else{
		free(copyLine);
		return SUCCESS;
	}
}

/* parse information of info words that need to be completed */
int parseInfoWord(InstructionCode **instructionCode, Symbol *symbols, int address, char *information){
	int ret, mode, value, dest = 0, src = 0, funct = 0, opcode = 0;
	char *operand = NULL, *lineNumber = NULL, attribute = 'R';

	operand = strtok(information, " \t");
	lineNumber = strtok(NULL, "\0");

	mode = getAddressingMode(operand);

	switch(mode){
		case DIRECT:
			value = getValue(symbols, operand);

			if(value < 0){
				printf("ERROR (line %s): Label '%s' is undefined.\n", lineNumber, operand);
				return FAIL;
			}

			/* symbol is external */
			ret = getValue(symbols, operand);

			if(ret == 0)
				attribute = 'E';

			prepareWord(value, &dest, &src, &funct, &opcode);

			completeWord(instructionCode, address, dest, src, funct, opcode, attribute);
			
			break;

		case RELATIVE:
			operand++;

			value = getValue(symbols, operand);

			if(value < 0){
				printf("ERROR (line %s): Label '%s' is undefined.\n", lineNumber, operand);
				return FAIL;
			}

			ret = getValue(symbols, operand);

			/* symbol is external */
			if(ret == 0){
				printf("ERROR (line %s): Relative addressing mode not compatible with external symbol.\n", lineNumber);
			}
			else
				attribute = 'A';

			value -= address;

			prepareWord(value, &dest, &src, &funct, &opcode);

			completeWord(instructionCode, address, dest, src, funct, opcode, attribute);
			
			break;
	}

	return SUCCESS;
}