DEBUG_FLAG= -DDEBUG -g -Wall
CFLAGS=-DDEBUG -g -Wall

.SUFFIXES: .cpp .o

.cpp.o:
	g++ $(CFLAGS) -c $<


PIPESIM: Pipesim.o Pipeline.o DependencyChecker.o Instruction.o OpcodeTable.o RegisterTable.o ASMParser.o MachLangParser.o StallPipeline.o DataForwardPipeline.o
	g++ -o PIPESIM Pipesim.o Pipeline.o DependencyChecker.o Instruction.o OpcodeTable.o RegisterTable.o ASMParser.o MachLangParser.o StallPipeline.o DataForwardPipeline.o

DependencyChecker.o: DependencyChecker.h OpcodeTable.h RegisterTable.h Instruction.h

MachLangParser.o: MachLangParser.h OpcodeTable.h RegisterTable.h Instruction.h 

ASMParser.o: ASMParser.h OpcodeTable.h RegisterTable.h Instruction.h 

Instruction.o: OpcodeTable.h RegisterTable.h Instruction.h 

OpcodeTable.o: OpcodeTable.h 

RegisterTable.o: RegisterTable.h  

Pipeline.o: Pipeline.h 

StallPipeline.o: StallPipeline.h Pipeline.h

DataForwardPipeline.o: DataForwardPipeline.h Pipeline.h

clean:
	/bin/rm -f PIPESIM *.o core
