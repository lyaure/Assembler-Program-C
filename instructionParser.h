#ifndef INSTRCTION_PARSER_H
#define INSTRCTION_PARSER_H


#include "instructionCode.h"
#include "symbolsList.h"
#include "instructionsTable.h"



enum directive {DATA, STRING, EXTERN, ENTRY};
enum ParseStatus {START, DIGIT, COMMA, OPERATOR, OPERAND1, OPERAND2};
enum instructionsName {MOV, CMP, ADD, SUB, LEA, CLR, NOT, INC, DEC, JMP, BNE, JSR, RED, PRN, RTS, STOP};
enum numOfOperands {NO_OPERANDS, ONE_OPERANDS, TWO_OPERANDS};


int parseStr(const char *line, const int lineNumber, InstructionCode **instructionCode, int *IC, Symbol **symbols, 
				InstructionCode **dataTable, int *DC, int continueToInsert);
int secondParse(const char *line, const int lineNumber, InstructionCode **instructionCode, int *IC, Symbol **symbols);
int isStrEmpty(const char *str);
int parseLine(const char *str, const int lineNumber, InstructionCode **instructionCode, int *IC, Symbol **symbols, InstructionCode **dataTable, 
						int *DC, int continueToInsert);
void removePaddingSpaces(const char *str, char **newStr);
int isLabelLegal(char *label, const int lineNumber);
int parseDataDirective(char *data, InstructionCode **dataTable, int *DC, const int lineNumber, int continueToInsert);
int parseStringDirective(char *data, InstructionCode **dataTable, int *DC, const int lineNumber, int continueToInsert);
int parseInstruction(char *instruction, InstructionCode **instructionCode, int *IC, const int lineNumber, int continueToInsert);
int checkIfRegister(char *str);
int getAddressingMode(char *str);
int insertInfoWord(InstructionCode **instructionCode, Instruction *instructionTable, int instructionNumber, char *operand, int *IC, int continueToInsert, int lineNumber);
int immediateMode(InstructionCode **instructionCode, Instruction *instructionTable, int instructionNumber, char *token, int *IC, int continueToInsert, int lineNumber);
int directMode(InstructionCode **instructionCode, Instruction *instructionTable, int instructionNumber, char *token, int *IC, int continueToInsert, int lineNumber);
int relativeMode(InstructionCode **instructionCode, Instruction *instructionTable, int instructionNumber, char *token, int *IC, int continueToInsert, int lineNumber);
int registerDirectMode(InstructionCode **instructionCode, Instruction *instructionTable, int instructionNumber, char *token, int *IC, int continueToInsert, int lineNumber);
int checkImmediateOp(char *operand, int *number, int lineNumber);
void prepareWord(int num, int *dest, int *src, int *funct, int *opcode);
void fillVariable(int num, unsigned int *mask, int *var, int varSize);
int parseOperands(char *token, char **operand1, char **operand2, int lineNumber);
int searchForEntryDirective(const char *line, const int lineNumber, InstructionCode **instructionCode, int *IC, Symbol **symbols);
int parseInfoWord(InstructionCode **instructionCode, Symbol *symbols, int address, char *information);



#endif