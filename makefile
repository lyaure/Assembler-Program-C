assembler: main.o firstRun.o secondRun.o instructionParser.o instructionCode.o instructionsTable.o symbolsList.o fileWriter.o
	gcc -g -ansi -Wall -pedantic main.o firstRun.o secondRun.o instructionParser.o instructionCode.o instructionsTable.o symbolsList.o fileWriter.o -o assembler -lm

main.o: main.c firstRun.h secondRun.h fileWriter.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o

firstRun.o: firstRun.c firstRun.h instructionParser.h instructionCode.h symbolsList.h
	gcc -c -ansi -Wall -pedantic firstRun.c -o firstRun.o

secondRun.o: secondRun.c secondRun.h instructionParser.h instructionCode.h symbolsList.h
	gcc -c -ansi -Wall -pedantic secondRun.c -o secondRun.o

instructionParser.o: instructionParser.c instructionParser.h instructionsTable.h instructionCode.h symbolsList.h globals.h
	gcc -c -ansi -Wall -pedantic instructionParser.c -o instructionParser.o 

instructionCode.o: instructionCode.c instructionCode.h
	gcc -c -ansi -Wall -pedantic instructionCode.c -o instructionCode.o

instructionsTable.o: instructionsTable.c instructionsTable.h
	gcc -c -ansi -Wall -pedantic instructionsTable.c -o instructionsTable.o

symbolsList.o: symbolsList.c symbolsList.h globals.h
	gcc -c -ansi -Wall -pedantic symbolsList.c -o symbolsList.o

fileWriter.o: fileWriter.c fileWriter.h instructionCode.h
	gcc -c -ansi -Wall -pedantic fileWriter.c -o fileWriter.o