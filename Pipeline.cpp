#include "Pipeline.h"

Pipeline::Pipeline(string inputFile) {

    Instruction i;
    myFormatCorrect = true;

    // determines the file type
    string fileType = inputFile.substr(inputFile.find(".") +1);
    
    // ASMParser reads the file, stores the instructions in myPipeline
    // and prints the RAW dependencies
    if (fileType == "asm") {

        ASMParser *parser;
        DependencyChecker checker;

        parser = new ASMParser(inputFile);

        i = parser->getNextInstruction(); 

        while (i.getOpcode() != UNDEFINED) {

            checker.addInstruction(i);
            myPipeline.push_back(i);
            i = parser->getNextInstruction();   
        }

        checker.printRAWDependences();

        delete parser;   
    }
    

    // MachLangParser reads the file, stores the instructions in myPipeline
    // and prints the RAW dependencies
    else if (fileType == "mach") {

        MachLangParser *parser;
        DependencyChecker checker;

        parser = new MachLangParser(inputFile);

        i = parser->getNextInstruction();  

        while (i.getOpcode() != UNDEFINED) {

            checker.addInstruction(i);
            myPipeline.push_back(i);
            i = parser->getNextInstruction();
        }

        checker.printRAWDependences();

        delete parser;
    }
    
    // wrong format if file is not type .mach or .asm
    else {

        myFormatCorrect = false;
    }

}

// executes the ideal pipeline
void Pipeline::execute() {

    DependencyChecker checker;

    int cycles = NUM_STAGES;

    // each instruction in myPipeline will have one extra cycle
    // because they start right after the previous instruction fetches
    for (int i = 0; i < myPipeline.size(); i++) {
        completionTimes.push_back(cycles);
        cycles++;
    }      

}

// prints the ideal pipeline
void Pipeline::print() {

    cout << "IDEAL:" << endl;

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

bool Pipeline::isFormatCorrect() {

    return myFormatCorrect;
}
