#include <iostream>
#include <string>
#include "register.h"

class OS;
class CPU {
private:
    // Private member variables
    register* registerFile;
    register PC;

public:
    // Constructors
    CPU(); // Default constructor
    CPU(register* registerFile, register PC); // Parameterized constructor
    // Destructor
    ~CPU();
    SetUpRegisters(); //names and adds each register to the previously empty register file
}

CPU::CPU() {
    PC = Register(0x400, "PC");
    register* registerFile[32];
    SetUpRegisters();
}

CPU::SetUpRegisters(){
    std::string names[32] = {
        "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
        "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
        "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
        "s8", "s9", "k0", "k1", "gp", "sp", "fp", "ra"
    };
    //initialize all of the register names
    for (int i = 0; i < 32; i++) {
        registerFile[i] = Register(0x00, names[i]);
    }
    registerFile[29].setAddress(0x3400); //29 is the stack pointer

}

//todo:
//one function that takes in an instruction
//and determines the registers it needs, the instruction 
//based on the opcode

CPU::PerformInstruction(uint32_t instruction){
    std::unorderedmap<int, void(*)>instructionList;
    int reg_a, reg_b, reg_c = 0;
    int opcode = (instruction >> (25) & 0x3F);
    if(opcode == 4){
        //r type instruction
        reg_a  = (instruction >> (21) & 0x1F); //get next five bits
        reg_b = (instruction >> (16) & 0x1F); //get next five bits
        reg_c = (instruction >> (11) & 0x1F) //get next five bits
        int shift_value = (instruction >> (6) & 0x1F); //get next five bits 0x1f: 00011111
        int function = (instruction & 0x3F) //0x3f looks like so: 00111111
        //hashtable with function pointers
        
        instructionList[0]= &ShiftRightArithmetic;//function pointer
        instructionList[3]= &Subtract; 
        instructionList[13] = &Add;
        instructionList[19] = &SetLessThan;
        instructionList[24] = &Or_;
        instructionList[25] = &Nor_;
        instructionList[31] = &And_;
        instructionList[33] = &JumpRegister;
        instructionList[35] = &ShiftRightLogical;
        instructionList[40] = &ShiftLeftLogical;
        if (instructionList.find(function) != instructionList.end()) {
            if(function == 0 || function == 35 || function == 40){
                instructionList[function](reg_b, reg_c, shift_value); //for shifts
            }
            else if(function == 33){
                instructionList[function](reg_a);//for jump register
            }
            else{
                instructionList[function](reg_a, reg_b, reg_c); // Call the function pointer
            }
             
            //there may be a problem here however, in that the shift value is only used in shift and will give the warning 
            //"unused variable"
        } else {
            std::cout << "Function not found for choice " << function << std::endl; //for debug
            PC +=4;  //if opcode is wrong PC+=4
        }

    }
    else if(opcode ==  56||opcode == 50||opcode == 46||opcode == 37||opcode == 28||opcode == 23||opcode == 16|| opcode == 9||opcode == 0){
       //r type instruction
        reg_a  = (instruction >> (21) & 0x1F); //get next five bits
        reg_b = (instruction >> (16) & 0x1F); //get next five bits
        reg_c = (instruction >> (11) & 0x1F) //get next five bits
        int immediate = (instruction & 0xFFFF); //get next 16 bits 0xFFF: 00001111111111111111
        
        //hashtable with function pointers
        instructionList[0]= &storeWord;//function pointer
        instructionList[9]= &addImm; //if opcode is wrong PC+=4
        instructionList[16] = &LoadByteUnsigned;
        instructionList[23] = &Jump;
        instructionList[28] = &BranchNotEqual;
        instructionList[37] = &BranchEqual;
        instructionList[46] = &StoreByte;
        instructionList[50] = &JumpAndLink;
        instructionList[56] = &LoadWord;
        if (instructionList.find(function) != instructionList.end()) {
            instructionList[opcode](reg_a, reg_b reg_c, immediate); // Call the function pointer
        } else {
            std::cout << "Function not found for choice " << opcode << std::endl;
            PC +=4;  //should never go off
        }
    }
    else{
        PC +=4;
    }
    //todo: separate all of the bits depending on the opcode
    //if opcode == 4 then i type, and seperate bits based on that
    //if opcode == 56, 50, 46, 37, 28, 23, 16,9, or 0 r type
    //else increment the PC by 4
    //this code is not dry at all but i cant really think of a way to make it better
}

void ShiftRightArithmetic(int reg_b, int reg_c, int shift){
    registerFile[reg_c] = registerFile[reg_b] >> shift_value;
}//function pointer

void Subtract(int reg_a, int reg_b, int reg_c){
    registerFile[reg_c] = registerFile[reg_a] - registerFile[reg_b];
}

void Add(int reg_a, int reg_b, int reg_c){
    registerFile[reg_c] = registerFile[reg_a] + registerFile[reg_b];
}

void SetLessThan(int reg_a, int reg_b, int reg_c,){
    registerFile[reg_c] = (registerFile[reg_a] < registerFile[reg_b]);
}

void Or_(int reg_a, int reg_b, int reg_c){
    registerFile[reg_c] = registerFile[reg_a] | registerFile[reg_b];
}

void Nor_(int reg_a, int reg_b, int reg_c){
    registerFile[reg_c] = ~(registerFile[reg_a] | registerFile[reg_b]);
}

void And_(int reg_a, int reg_b, int reg_c){
    registerFile[reg_c] = registerFile[reg_a] & registerFile[reg_b];
}

void JumpRegister(int reg_a){
    PC+= R[reg_a];
}

        instructionList[3]= &Subtract; 
        instructionList[13] = &Add;
        instructionList[19] = &SetLessThan;
        instructionList[24] = &Or_;
        instructionList[25] = &Nor_;
        instructionList[31] = &And_;
        instructionList[33] = &JumpRegister;
        instructionList[35] = &ShiftRightLogical;
        instructionList[40] = &ShiftLeftLogical;


