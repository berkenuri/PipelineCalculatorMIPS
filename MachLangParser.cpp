#include "MachLangParser.h"
#include <bitset>
#include <string>
#include <sstream>

// checks the length and the syntatic correctness of the input file line
bool MachLangParser::checkLine(string line) {

    if (line.size() > 32)
        return false;

    Opcode o = opcodes.getOpcode(line.substr(0, 6), line.substr(26, 6)); 
    
    if(o == UNDEFINED)
        return false;
              
    return true;
}

// deconstructor
MachLangParser::~MachLangParser() {}

// returns true if the file specified was syntactically correct.  Otherwise,
// false
bool MachLangParser::isFormatCorrect() {
    
    return myFormatCorrect;
}

// reads the input file, checks for syntax, translates each binary encoded line 
// into MIPS assembly instruction, stores list of instructions to iterate over.
MachLangParser::MachLangParser(string input_file) {

    Instruction i;
    myFormatCorrect = true;

    ifstream in;
    in.open(input_file.c_str());
	
    if (in.bad()) {

        myFormatCorrect = false;
    } 
	
    else {
	        
        string line;	
        	        
        while (getline(in, line)) {

            // checks for syntax
            if (!checkLine(line)) {
                myFormatCorrect = false; 
                break;
            }

            // decodes the binary encoding
            decode(i, line);  

            // stores the decoded instruction into string
            string assembled = assemble(i, line);

            // assembles string to instruction's proper format
            i.setAssembly(assembled);

            // stores and iterates over the instructions
            myInstructions.push_back(i);
        }
    }

    myIndex = 0;
}

Instruction MachLangParser::getNextInstruction() {      
    // Iterator that returns the next Instruction in the list of Instructions.    
    if(myIndex < (int)(myInstructions.size())) {
  
        myIndex++;    
        return myInstructions[myIndex-1];     
    }

    Instruction i;
   
    return i;
}

// gets the decoded instruction and assembles into a string
// with respect to its type's format
string MachLangParser::assemble(Instruction &i, string line) { 

    // gets the proper opcode for each line
    Opcode o = opcodes.getOpcode(line.substr(0, 6), line.substr(26, 6)); 

    string decoded;

    // assembles R types
    if (opcodes.getInstType(o) == RTYPE) {

        decoded += assembleR(i, o);
    }

    // assembles I types
    else if (opcodes.getInstType(o) == ITYPE) {

        decoded += assembleI(i, o);
    }

    // assembles J types
    else {
     
        decoded += assembleJ(i, o);
    }

    return decoded;
}

// assembles R types into their proper format
string  MachLangParser::assembleR(Instruction &i, Opcode o) { 

    string decoded; 
    decoded += opcodes.getName(o);
       
    if (opcodes.RDposition(o) != -1) {
       
        if (opcodes.RDposition(o) == 0)            
            decoded += " ";
   
        decoded += registers.getName(i.getRD()); 
        

        if (opcodes.RDposition(o) != (opcodes.numOperands(o) -1) )
            decoded += ", " ;    
    }    

    if (opcodes.RSposition(o) != -1) {
     
        if (opcodes.RSposition(o) == 0)   
            decoded += "\t";

        decoded += registers.getName(i.getRS()); 
         
        if (opcodes.RSposition(o) != (opcodes.numOperands(o) -1) )
            decoded += ", " ;
    }

    if (opcodes.RTposition(o) != -1) {

        if (opcodes.RTposition(o) == 0)
            decoded += " ";

        decoded += registers.getName(i.getRT()); 
           
        if (opcodes.RTposition(o) != (opcodes.numOperands(o) -1) )           
            decoded += ", " ;    
    }

    if (opcodes.IMMposition(o) != -1) {

        if (opcodes.IMMposition(o) == 0)
            decoded += " ";

        stringstream ss;
        ss << i.getImmediate();
        string immediate = ss.str();
        decoded += immediate;
           
        if (opcodes.IMMposition(o) != (opcodes.numOperands(o) -1) )
            decoded += ", " ;   
    }

    return decoded;
}

// assembles I types into their proper format
string MachLangParser::assembleI(Instruction &i, Opcode o) {

    string decoded;  
    decoded += opcodes.getName(o);

    if (decoded.compare("beq") == 0) {
        decoded += " " ;
        decoded += registers.getName(i.getRS());
        decoded += ", " ;
        decoded += registers.getName(i.getRT());
        decoded += ", 0x" ;
        stringstream stream;
        stream << hex << i.getImmediate() ;
        string immediate = stream.str();
        decoded += immediate;
    }

    else if (decoded.compare("lw") == 0) {
        decoded += " " ;
        decoded += registers.getName(i.getRT());
        decoded += ", " ;
        stringstream ss;
        ss << i.getImmediate();
        string immediate = ss.str();
        decoded += immediate;
        decoded += "(" ;
        decoded += registers.getName(i.getRS());
        decoded += ")" ;
    }

    else {

        if (opcodes.RTposition(o) != -1) {
        
            if (opcodes.RTposition(o) == 0)   
            decoded += " ";

            decoded += registers.getName(i.getRT()); 
         
            if (opcodes.RTposition(o) != (opcodes.numOperands(o) -1) )
            decoded += ", " ;
        }

        if (opcodes.RSposition(o) != -1) {

            if (opcodes.RSposition(o) == 0)
            decoded += " ";
        
            decoded += registers.getName(i.getRS()); 
              
            if (opcodes.RSposition(o) != (opcodes.numOperands(o) -1) )           
            decoded += ", " ;    
        }
    
        if (opcodes.IMMposition(o) != -1) {
        
            if (opcodes.IMMposition(o) == 0)
                decoded += " ";

            stringstream ss;
            ss << i.getImmediate();
            string immediate = ss.str();
            decoded += immediate;
           
            if (opcodes.IMMposition(o) != (opcodes.numOperands(o) -1) )
                decoded += ", " ;   
        }
    }

    return decoded;
}

// assembles J types into their proper format
string MachLangParser::assembleJ(Instruction &i, Opcode o) {

    string decoded;  
    decoded += opcodes.getName(o);
    decoded += " " ;
    decoded += "0x" ; 
    stringstream stream;
    stream << hex << (i.getImmediate() * 4) ;
    decoded += stream.str();

    return decoded;
}

// decodes the binary encodings to get a valid instruction by merging
//all other helper decode methods
void MachLangParser::decode(Instruction &i, string line) {
	
    Opcode o = opcodes.getOpcode(line.substr(0, 6), line.substr(26, 6)); 

    // decodes R type instructions
    if (opcodes.getInstType(o) == RTYPE) {

        decodeR(i, line, o);
    }
	
    // decodes J type instructions
    else if (opcodes.getInstType(o) == JTYPE) {

        decodeJ(i, line, o);	
    }

    // decodes I type instructions
    else {

        decodeI(i, line, o);	
    }

}

// decodes R types
void MachLangParser::decodeR(Instruction &i, string line, Opcode o) {

    // stores the binary encoded line
    i.setEncoding(line);

    bitset<RS_RT_RD_size> RSbits(line.substr(6, 5));
    int RS = (int)RSbits.to_ulong();

    bitset<RS_RT_RD_size> RTbits(line.substr(11, 5));
    int RT = (int)RTbits.to_ulong();

    bitset<RS_RT_RD_size> RDbits(line.substr(16, 5));
    int RD = (int)RDbits.to_ulong();

    bitset<RS_RT_RD_size> shiftBits(line.substr(21, 5));
    int imm = (int)shiftBits.to_ulong();

    // merges the decoded opcode bitfields into an instruction
    i.setValues(o, RS, RT, RD, imm);
}

// decodes I types
void MachLangParser::decodeI(Instruction &i, string line, Opcode o) {
   
    // stores the binary encoded line
    i.setEncoding(line);
        
    bitset<RS_RT_RD_size> RSbits(line.substr(6, 5));
    int RS = (int)RSbits.to_ulong();

    bitset<RS_RT_RD_size> RTbits(line.substr(11, 5));
    int RT = (int)RTbits.to_ulong();

    bitset<I_IMM_size> immBits(line.substr(16, 16));
    int imm = (int)immBits.to_ulong();
   
    // merges the decoded opcode bitfields into an instruction
    i.setValues(o, RS, RT, 32, imm);     
}

// decodes J types
void MachLangParser::decodeJ(Instruction &i, string line, Opcode o) {
	   
    // stores the binary encoded line
    i.setEncoding(line);
    
    bitset<J_IMM_size> immBits(line.substr(6,26));
    int imm = (int)immBits.to_ulong();

    // merges the decoded opcode bitfields into an instruction 
    i.setValues(o, 32, 32, 32, imm); 
}





































