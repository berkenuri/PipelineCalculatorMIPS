#include "DependencyChecker.h"

DependencyChecker::DependencyChecker(int numRegisters)
/* Creates RegisterInfo entries for each of the 32 registers and creates lists for
 * dependencies and instructions.
 */
{
  RegisterInfo r;

  // Create entries for all registers
  for(int i = 0; i < numRegisters; i++){
    myCurrentState.insert(make_pair(i, r));
  }
}

void DependencyChecker::addInstruction(Instruction i)
/* Adds an instruction to the list of instructions and checks to see if that 
 * instruction results in any new data dependencies.  If new data dependencies
 * are created with the addition of this instruction, appropriate entries
 * are added to the list of dependences.
 */
{
  InstType iType = i.getInstType();

  // got the instruction's opcode
  Opcode o = i.getOpcode();

  // if the instruction have the necessary registers
  // checks for register's corresponding dependence 
  switch(iType){
      
  case RTYPE:
    if (myOpcodeTable.RSposition(o) != -1)
        checkForReadDependence(i.getRS());

    if (myOpcodeTable.RDposition(o) != -1) 
        checkForWriteDependence(i.getRD());

    if (myOpcodeTable.RTposition(o) != -1)
        checkForReadDependence(i.getRT());
    break;

  case ITYPE:
    if (myOpcodeTable.RSposition(o) != -1)
        checkForReadDependence(i.getRS());

    if (myOpcodeTable.RTposition(o) != -1)
        checkForWriteDependence(i.getRT());    
    break;

  case JTYPE:
    // don't need to check for dependence 
    break;

  default:
    // do nothing
    break;
  }

  myInstructions.push_back(i);

}

void DependencyChecker::checkForReadDependence(unsigned int reg)
  /* Determines if a read data dependence occurs when reg is read by the current
   * instruction.  If so, adds an entry to the list of dependences. Also updates
   * the appropriate RegisterInfo entry regardless of dependence detection.
   */
{
  
    // initialized a new dependence
    Dependence dependenceEntry;

    // if the register is being read or written; assigns the dependence type,
    // adds the first instruction# the register was previously accessed and adds
    // the current instruction#, lastly adds the dependence to the list if it is 
    // not Read after Read
    if (myCurrentState[reg].accessType != A_UNDEFINED) {  

        dependenceEntry.registerNumber = reg;

        int lastInstruction = myCurrentState[reg].lastInstructionToAccess;
        dependenceEntry.previousInstructionNumber = lastInstruction;

        list<Instruction>::iterator liter;
        int currentInstruction = 0;
        
        for(liter = myInstructions.begin(); liter != myInstructions.end(); liter++)
            currentInstruction++;

        dependenceEntry.currentInstructionNumber = currentInstruction;

        if (myCurrentState[reg].accessType == WRITE) { 
           
            dependenceEntry.dependenceType = RAW;
        }

        if (myCurrentState[reg].accessType == READ) {
            
            dependenceEntry.dependenceType = D_UNDEFINED;  
        }
        
        if (dependenceEntry.dependenceType != D_UNDEFINED) {             
         
            myDependences.push_back(dependenceEntry);
        }
 
    }
    
    myCurrentState[reg].lastInstructionToAccess = myInstructions.size();
    myCurrentState[reg].accessType = READ;    
}


void DependencyChecker::checkForWriteDependence(unsigned int reg)
  /* Determines if a write data dependence occurs when reg is written by the current
   * instruction.  If so, adds an entry to the list of dependences. Also updates 
   * the appropriate RegisterInfo entry regardless of dependence detection.
   */
{
    // initialized a new dependence
    Dependence dependenceEntry;

    // if the register is being read or written; assigns the dependence type,
    // adds the first instruction# the register was previously accessed and adds
    // the current instruction#, lastly adds the dependence to the list if it is 
    // not Read after Read
    if (myCurrentState[reg].accessType != A_UNDEFINED) {  

        dependenceEntry.registerNumber = reg;

        int lastInstruction = myCurrentState[reg].lastInstructionToAccess;
        dependenceEntry.previousInstructionNumber = lastInstruction;

        list<Instruction>::iterator liter;
        int currentInstruction = 0;
        
        for(liter = myInstructions.begin(); liter != myInstructions.end(); liter++)
            currentInstruction++;

        dependenceEntry.currentInstructionNumber = currentInstruction;

        if (myCurrentState[reg].accessType == WRITE) {

            dependenceEntry.dependenceType = WAW;   
        }

        if (myCurrentState[reg].accessType == READ) { 
            
            dependenceEntry.dependenceType = WAR;
        }
        
        if (dependenceEntry.dependenceType != D_UNDEFINED) {
         
            myDependences.push_back(dependenceEntry);
        }
       
    }

    myCurrentState[reg].lastInstructionToAccess = myInstructions.size();
    myCurrentState[reg].accessType = WRITE;
}

// if the parameter instruction has RAW dependence, returns previous instruction number 
int DependencyChecker::getRAWInstNumber(int instNumber) {

    list<Dependence>::iterator diter;

    for (diter = myDependences.begin(); diter != myDependences.end(); diter++) {

        if ( ((*diter).dependenceType == RAW) && ((*diter).currentInstructionNumber == instNumber) )
            return (*diter).previousInstructionNumber;
    }
 
    return -1;
}

// prints the RAW dependences
void DependencyChecker::printRAWDependences() {

    list<Dependence>::iterator diter;

    for (diter = myDependences.begin(); diter != myDependences.end(); diter++){
        
        // if there is a RAW dependence
        if( (*diter).dependenceType == RAW) {
        
            cout << "RAW Dependence between instruction ";  
              
            list<Instruction>::iterator liter;

            // print the instructions that are the dependence's previous and current instructions
            int i = 0;
            for (liter = myInstructions.begin(); liter != myInstructions.end(); liter++) {

                if ( (*diter).previousInstructionNumber == i ) 
                    cout << i <<  " " << (*liter).getAssembly() <<  " and ";
                
                if ( (*diter).currentInstructionNumber == i ) 
                    cout << i <<  " " << (*liter).getAssembly() << endl;

                i++;
            }

        }
    }
}




void DependencyChecker::printDependences()
  /* Prints out the sequence of instructions followed by the sequence of data
   * dependencies.
   */ 
{
  // First, print all instructions
  list<Instruction>::iterator liter;
  int i = 0;
  cout << "INSTRUCTIONS:" << endl;
  for(liter = myInstructions.begin(); liter != myInstructions.end(); liter++){
    cout << i << ": " << (*liter).getAssembly() << endl;
    i++;
  }

  // Second, print all dependences
  list<Dependence>::iterator diter;
  cout << "DEPENDENCES: \nType Register (FirstInstr#, SecondInstr#) " << endl;
  for(diter = myDependences.begin(); diter != myDependences.end(); diter++){
    switch( (*diter).dependenceType){
    case RAW:
      cout << "RAW \t";
      break;
    case WAR:
      cout << "WAR \t";
      break;
    case WAW:
      cout << "WAW \t";
      break;
    default:
      break;
    }

    cout << "$" << (*diter).registerNumber << " \t";
    cout << "(" << (*diter).previousInstructionNumber << ", ";
    cout << (*diter).currentInstructionNumber << ")" << endl;
  }


}
