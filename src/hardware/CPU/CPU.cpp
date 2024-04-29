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
    registerFile[29].setAddress(0x3400);

    
}
