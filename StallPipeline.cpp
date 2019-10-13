#include "StallPipeline.h"

using namespace std;

// the constructor is the same as parent class's
StallPipeline::StallPipeline(string inputFile) : Pipeline(inputFile) {}

void StallPipeline::execute() {

    Instruction inst;
    DependencyChecker checker;

    int cycles = NUM_STAGES;

    for (int i = 0; i < myPipeline.size(); i++) {

       int stall = 0;
        
       inst = myPipeline.at(i);
       checker.addInstruction(inst);

       // if the instruction is a control instruction, add stall
        if (i > 0) {
            if ( (myPipeline.at(i-1).getIMMLabel() == true) || (myPipeline.at(i-1).getInstType() == JTYPE) ) {  
             
                stall++;
            }
        }
   
        // when can't have a dependency in the first instruction,
        // our stall increases by (3 - difference of the dependent instruction numbers)
        // when there is a RAW dependence
        if (checker.getRAWInstNumber(i) > 0) {

            stall += 3 - (i - checker.getRAWInstNumber(i));
        }
     
        // add stalls to the total cycles
        cycles += stall;
        completionTimes.push_back(cycles);
        cycles++;
    }

}

// prints the stall pipeline
void StallPipeline::print() {

    cout << "STALL:" << endl;

    cout << "Instr# \t Completion Times \t Mnemonic" << endl;

    vector<Instruction>::iterator it;
   
    int instrNumber = -1;
    for (it = myPipeline.begin(); it != myPipeline.end(); it++){
       
        instrNumber++;

        cout << instrNumber << "\t" << completionTimes.at(instrNumber) 
            << "\t \t \t|" << (*it).getAssembly() << endl;
    }

    cout << "Total time is " << completionTimes.at(instrNumber) << endl << endl;

}

