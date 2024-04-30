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
    int opcode = (instruction >> (32-6) & 0x3F);
    int * rtype = { 56, 50, 46, 37, 28, 23, 16,9, 0}
    if(opcode == 4){
        //i type instruction

    }
    else if(opcode ==  56||opcode == 50||opcode == 46||opcode == 37||opcode == 28||opcode == 23||opcode == 16|| opcode == 9||opcode == 0){
        //r type
    }
    else{
        PC +=4;
    }

    //todo: separate all of the bits depending on the opcode
    //if opcode == 4 then i type, and seperate bits based on that
    //if opcode == 56, 50, 46, 37, 28, 23, 16,9, or 0 r type
    //else increment the PC by 4


}
