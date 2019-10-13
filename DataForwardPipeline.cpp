#include "DataForwardPipeline.h"

using namespace std;

// the constructor is the same as parent class's
DataForwardPipeline::DataForwardPipeline(string inputFile) : Pipeline(inputFile) {}

void DataForwardPipeline::execute() {

    Instruction inst;
    DependencyChecker checker;

    int cycles = NUM_STAGES;

    for (int i = 0; i < myPipeline.size(); i++) {

        int stall = 0;

        inst = myPipeline.at(i);
        checker.addInstruction(inst);

        // if the instruction is a control instruction or a memory type like load word, add stall
        if (i > 0) {
            if ( (myPipeline.at(i-1).getIMMLabel() == true) || (myPipeline.at(i-1).getInstType() == JTYPE) 
                    || myPipeline.at(i-1).getMemoryType() == true ) {  

                stall++;
            }
        }

        // add the stalls to the total cycles
        cycles += stall;
        completionTimes.push_back(cycles);
        cycles++;
    }

}

// prints the data forward pipeline
void DataForwardPipeline::print() {

    cout << "FORWARDING:" << endl;

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


