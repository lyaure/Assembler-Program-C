#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include "instructionCode.h"
#include "symbolsList.h"


void writeFiles(char * filename, InstructionCode *instructions, Symbol *symbols, int IC, int DC);


#endif