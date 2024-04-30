#ifndef CPU_H
#define CPU_H
#pragma once

#include "register.h" // Include the header file for the Register class

class OS;
class CPU {
private:
    // Private member variables
    std::vector<Register> registerFile; // Pointer to an array of Register objects
    Register PC;

public:
    // Constructors
    CPU(); // Default constructor
    CPU(Register* registerFile, Register PC); // Parameterized constructor

    // Destructor
    ~CPU();

    SetUpRegisters()
    
};

#endif // CPU_H
