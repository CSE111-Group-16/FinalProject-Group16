#ifndef CPU_H
#define CPU_H
#pragma once

#include "register.h" // Include the header file for the Register class
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <any>


class OS;
class CPU {
public:
    // Constructors
    //CPU(); // Default constructor
    //CPU(register* registerFile, register PC); // Parameterized constructor
    // Destructor
    // ~CPU();
    CPU(OS* set_os) : os(set_os) {}; // Default constructor
    OS* os;

    
    void SetUpRegisters(); //names and adds each register to the previously empty register file
    void PerformInstruction(const uint32_t instruction);

    //rtype
    void ShiftRightArithmetic(int16_t reg_b, int16_t reg_c, int8_t shift);
    void ShiftRightLogical(int16_t reg_b, int16_t reg_c, int8_t shift); 
    void ShiftLeftLogical(int16_t reg_b, int16_t reg_c, int8_t shift);
    void Subtract(int16_t reg_a, int16_t reg_b, int16_t reg_c);
    void Add(int16_t reg_a, int16_t reg_b, int16_t reg_c);
    void SetLessThan(int16_t reg_a, int16_t reg_b, int16_t reg_c);
    void Or_(int16_t reg_a, int16_t reg_b, int16_t reg_c);
    void Nor_(int16_t reg_a, int16_t reg_b, int16_t reg_c);
    void And_(int16_t reg_a, int16_t reg_b, int16_t reg_c);
    void JumpRegister(int16_t reg_a);

    //itype
    void storeWord(int16_t reg_b, int16_t reg_c, int16_t immediate);
    void addImm(int16_t reg_a, int16_t reg_b, int16_t immediate);
    void LoadByteUnsigned(int16_t reg_a, int16_t reg_b, int16_t immediate);
    void Jump(int16_t immediate);
    void BranchNotEqual(int16_t reg_a, int16_t reg_b, int16_t immediate);
    void BranchEqual(int16_t reg_a, int16_t reg_b, int16_t immediate);
    void StoreByte(int16_t reg_a, int16_t reg_b, int16_t immediate);
    void JumpAndLink(int16_t immediate);
    void LoadWord(int16_t reg_a, int16_t reg_b, int16_t immediate);
};

#endif // CPU_H
