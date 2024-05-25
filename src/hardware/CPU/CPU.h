#ifndef CPU_H
#define CPU_H
#pragma once

#include "register.h"
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <any>
#include <cassert>

enum shift{
    opcode_shift = 26,
    reg_a_shift = 21,
    reg_b_shift = 16,
    reg_c_shift = 11,
    shift_shift = 6,
    byte_shift = 8,
    five_bitmask = 0x1F,
    six_bitmask = 0x3F,
    eight_bitmask = 0xFF,
    sixteen_bitmask = 0xFFFF,
    opcode_max = 64,
    rtype = 4,
    LW = 56,
    JAL = 50,
    SB = 46,
    BEQ = 37,
    BNE = 28,
    J = 23,
    LBU = 16,
    ADDI = 9,
    SW = 0,
    register_ra = 31,
    register_sp = 29,
    top_of_stack = 0x3400
};


class OS;
class CPU {
public:
    // Constructors
    CPU(OS* set_os) : os(set_os) {
        std::string names[32] = {
            "$0", "at", "v0", "v1", "a0", "a1", "a2", "a3",
            "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
            "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
            "s8", "s9", "k0", "k1", "gp", "sp", "fp", "ra"
        };
        //initialize all of the register names
        for (int i = 0; i < 32; i++) {
            registerFile[i] = Register(0x00, names[i]);
        }

        // setup i type instructions
        i_type_instructions_.emplace(0, &CPU::storeWord); //[0] = &storeWord;
        i_type_instructions_.emplace(9, &CPU::addImm); //[9] = &addImm //if opcode is wrong PC+=4
        i_type_instructions_.emplace(16, &CPU::LoadByteUnsigned); //[16] = &LoadByteUnsigned;
        i_type_instructions_.emplace(23, &CPU::Jump); //[23] = &Jump;
        i_type_instructions_.emplace(28, &CPU::BranchNotEqual); //[28] = &BranchNotEqual;
        i_type_instructions_.emplace(37, &CPU::BranchEqual); //[37] = &BranchEqual;
        i_type_instructions_.emplace(46, &CPU::StoreByte); //[46] = &StoreByte;
        i_type_instructions_.emplace(50, &CPU::JumpAndLink); //[50] = &JumpAndLink;
        i_type_instructions_.emplace(56, &CPU::LoadWord); //[56] = &LoadWord;

        // setup r type instructions
        r_type_instructions_.emplace(0, &CPU::ShiftRightArithmetic); //[0]= &ShiftRightArithmetic;
        r_type_instructions_.emplace(3, &CPU::Subtract); //[3]= &Subtract; 
        r_type_instructions_.emplace(13, &CPU::Add); //[13] = &Add;
        r_type_instructions_.emplace(19, &CPU::SetLessThan); //[19] = &SetLessThan;
        r_type_instructions_.emplace(24, &CPU::Or_); //[24] = &Or_;
        r_type_instructions_.emplace(25, &CPU::Nor_); //[25] = &Nor_;
        r_type_instructions_.emplace(31, &CPU::And_); //[31] = &And_;
        r_type_instructions_.emplace(33, &CPU::JumpRegister); //[33] = &JumpRegister;
        r_type_instructions_.emplace(35, &CPU::ShiftRightLogical); //[35] = &ShiftRightLogical;
        r_type_instructions_.emplace(40, &CPU::ShiftLeftLogical); //[40] = &ShiftLeftLogical;
    }; 
    OS* os;
    Register registerFile[32];
    uint16_t PC; // program counter


    //CPU(Register* registerFile, Register PC); // Parameterized constructor
    void resetStackPointer();
    //void SetUpRegisters(); //names and adds each register to the previously empty register file
    void PerformInstruction(const uint32_t instruction);
    void initialJAL(uint32_t address_to_setup);
    // Destructor
private:
    typedef void (*funcPtr)(void);
    std::unordered_map<int, void (CPU::*)()>r_type_instructions_;
    std::unordered_map<int, void (CPU::*)()>i_type_instructions_;
    uint16_t reg_a_, reg_b_, reg_c_, immediate_value_;
    uint16_t shift_value_, function_value_;

    // debug options:
    bool logInstructionOperation = false; // logs operation of instruction (UNFINISHED)
    bool logInstructionBreakdown = false; // logs translation of instruction to parts
    bool logPreInstructionReg = false; // logs values stored at registers before instruction
    bool logPostInstructionReg = false; // logs values stored at registers after instructions
    bool logFullRegisters = false;
    bool logInstructionName = false;
    bool logStdout = false;
    bool logStderr = false;
    bool logStdin = false;

    // debug helper functions:
    void logRegisters(bool reg_a=false, bool reg_b=false, bool reg_c=false, bool reg_pc=false);
    void logAllRegisters();
    void i_typeBreakdown(uint32_t);
    void r_typeBreakdown(uint32_t);

    // r-type instructions:
    void ShiftRightArithmetic();
    void ShiftRightLogical(); 
    void ShiftLeftLogical();
    void Subtract();
    void Add();
    void SetLessThan();
    void Or_();
    void Nor_();
    void And_();
    void JumpRegister();

    // i-type instructions:
    void storeWord();
    void addImm();
    void LoadByteUnsigned();
    void Jump();
    void BranchNotEqual();
    void BranchEqual();
    void StoreByte();
    void JumpAndLink( );
    void LoadWord();
};

#endif // CPU_H

