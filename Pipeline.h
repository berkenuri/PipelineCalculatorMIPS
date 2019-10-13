#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include "DependencyChecker.h"
#include "MachLangParser.h"
#include "ASMParser.h"

#include <iostream>
#include <string>
#include <cstddef>

using namespace std;

/* The stages in the pipeline in the order they appear in the datapath */
enum PipelineStages{
  FETCH, 
  DECODE, 
  EXECUTE, 
  MEMORY, 
  WRITEBACK, 
  NUM_STAGES
};


/* This class simulates an ideal pipeline, executing the instructions 
 * from an input file on an ideal pipeline and printing out information 
 * about RAW dependences, the completion times of the instructions in 
 * the order they appear in the file, and the total execution time for
 * the instructions from the file.
 */
class Pipeline{
 public:

  /* The constructor is passed either the name of a MIPS assembly file or
   * the name of a MIPS machine instruction file.  If the input file is
   * not a valid input file (due to syntactical errors or unsupported
   * instructions being specified), the format is considered incorrect.
   */
  Pipeline(string inputFile);

  /* Executes the instructions passed into the constructor.  The method
   * determines the dependences and calculates the completion time of each
   * instruction and the overall execution time for the set of instructions,
   * storing that information for later printing.
   */
  virtual void execute();

  /* Prints the pipeline type, prints the RAW dependences found in the 
   * instructions, prints each instruction with its completion time, and
   * prints the total execution time.
   */
  virtual void print();

  /* Returns true if the input file was syntactically correct and only 
   * contained supported instructions.  Otherwise, returns false.
   */
  bool isFormatCorrect();

 protected:

  bool myFormatCorrect;

  // the vector working as a pipeline, storing instructions
  vector<Instruction> myPipeline;

  // the vector of each instruction in the pipeline's 
  // completion times
  vector<int> completionTimes;

  OpcodeTable myOpcodeTable; 
  
};

#endif
