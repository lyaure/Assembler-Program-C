#ifndef FIRST_RUN_H
#define FIRST_RUN_H


#include "instructionParser.h"

#define TABLE_MIN_SIZE 2

int firstRun(FILE *file, InstructionCode **instructions, int *IC, Symbol **symbols, InstructionCode **data, int *DC);


#endif