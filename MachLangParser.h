#ifndef __MACHLANGPARSER_H__
#define __MACHLANGPARSER_H__

using namespace std;

#include <iostream>
#include <fstream>
#include "Instruction.h"
#include "RegisterTable.h"
#include "OpcodeTable.h"
#include <math.h>
#include <vector>
#include <sstream>
#include <stdlib.h>

/* This class reads in a machine language representation of a MIPS
 * program.  If the file is correctly formatted (i.e. every line
 * contains a properly formatted 32 bit machine language/binary
 * representation of a supported MIPS instruction, this
 * class will retain a list of Instructions (one for each machine
 * language instruction from the file).  This list of Instructions
 * can be iterated through.
 */
class MachLangParser {

  public:

  // Specify a text file containing machine language/binary representations
  // of MIPS instructions.  This constructor checks the correctness of 
  // each line of the file to see if it is a 32 bit binary sequence that
  // represents a supported MIPS instruction.  If it does, a list of
  // Instructions is created and stored internally.  If it does not, then
  // the class will indicate that the file is incorrectly formatted.
  //
  MachLangParser(string input_file);

  // Destructor
  ~MachLangParser();

  // Returns true if the file specified was a valid/correct MIPS instruction
  // file.  Otherwise, returns false.
  bool isFormatCorrect();

  // Iterator that returns the next Instruction in the list of Instructions.
  Instruction getNextInstruction();

  
  private:

    // constant integers used to separate binary encodings' bitfields
    static const int RS_RT_RD_size = 5;
    static const int I_IMM_size = 16;
    static const int J_IMM_size = 22;

    vector<Instruction> myInstructions;      // list of Instructions
    int myIndex;                             // iterator index
    bool myFormatCorrect;

    RegisterTable registers;                 // encodings for registers
    OpcodeTable opcodes;                     // encodings of opcodes
    int myLabelAddress;   // Used to assign labels addresses


    // checks the length and the syntatic correctness of the input file line
    bool checkLine(string line); 

    // decodes the binary encodings to get a valid instruction by merging
    // all other helper decode methods
    void decode(Instruction &i, string line);

    // decodes R types
    void decodeR(Instruction &i, string line, Opcode o);

    // decodes I types
    void decodeI(Instruction &i, string line, Opcode o);

    // decodes J types
    void decodeJ(Instruction &i, string line, Opcode o);

    // gets the decoded instruction and assembles into a string
    // with respect to its type's format
    string assemble(Instruction &i, string line);

    // assembles R types into their proper format
    string assembleR(Instruction &i, Opcode o);

    // assembles I types into their proper format
    string assembleI(Instruction &i, Opcode o);

    // assembles J types into their proper format
    string assembleJ(Instruction &i, Opcode o);
};

#endif
