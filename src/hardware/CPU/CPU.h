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
    CPU(OS* set_os) : os(set_os) {}; // Default constructor
    OS* os;
    //CPU(Register* registerFile, Register PC); // Parameterized constructor

    // Destructor
<<<<<<< HEAD
    //~CPU();
    void SetUpRegisters(); //names and adds each register to the previously empty register file

=======
    ~CPU();

    SetUpRegisters();
    
>>>>>>> CPU
};

#endif // CPU_H
