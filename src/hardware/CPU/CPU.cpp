#include <iostream>
#include <string>
#include <unordered_map>
#include <any>
#include "register.h"

public:
    // Constructors
    CPU(); // Default constructor
    CPU(register* registerFile, register PC); // Parameterized constructor
    // Destructor
    ~CPU();
    SetUpRegisters(); //names and adds each register to the previously empty register file
    PerformInstruction();

    //rtype
    ShiftRightArithmetic(int16_t reg_b, int16_t reg_c, int8_t shift);
    ShiftRightLogical(int16_t reg_b, int16_t reg_c, int8_t shift); 
    ShiftLeftLogical(int16_t reg_b, int16_t reg_c, int8_t shift);
    Subtract(int16_t reg_a, int16_t reg_b, int16_t reg_c);
    Add(int16_t reg_a, int16_t reg_b, int16_t reg_c);
    SetLessThan(int16_t reg_a, int16_t reg_b, int16_t reg_c);
    Or_(int16_t reg_a, int16_t reg_b, int16_t reg_c);
    Nor_(int16_t reg_a, int16_t reg_b, int16_t reg_c);
    And_(int16_t reg_a, int16_t reg_b, int16_t reg_c);
    JumpRegister(int16_t reg_a);

    //itype
    storeWord(int16_t reg_b, int16_t reg_c, int16_t immediate);
    addImm(int16_t reg_a, int16_t reg_b, int16_t immediate);
    LoadByteUnsigned(int16_t reg_a, int16_t reg_b, int16_t immediate);
    Jump(int16_t immediate);
    BranchNotEqual(int16_t reg_a, int16_t reg_b, int16_t immediate);
    BranchEqual(int16_t reg_a, int16_t reg_b, int16_t immediate);
    StoreByte(int16_t reg_a, int16_t reg_b, int16_t immediate);
    JumpAndLink(int16_t immediate);
    LoadWord(int16_t reg_a, int16_t reg_b, int16_t immediate);
    
}

CPU::CPU() : PC(0x400, "PC") {
    registerFile = new register[32];
    SetUpRegisters(); // Initialize register names and addresses
}

CPU::CPU(register* registerFile, register PC) : PC(PC), registerFile(registerFile) {
    SetUpRegisters(); // Initialize register names and addresses
}

CPU::~CPU() {
    delete[] registerFile; // Free the dynamically allocated registerFile array
}

void CPU::SetUpRegisters(){
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

void CPU::PerformInstruction(const uint32_t& instruction){
    std::unorderedmap<int, std::any>instructionList;
    int16_t reg_a, reg_b, reg_c = 0;
    int opcode = (instruction >> (25) & 0x3F);
    if(opcode == 4){
        //r type instruction
        reg_a  = (instruction >> (21) & 0x1F); //get next five bits
        reg_b = (instruction >> (16) & 0x1F); //get next five bits
        reg_c = (instruction >> (11) & 0x1F) //get next five bits
        int8_t shift = (instruction >> (6) & 0x1F); //get next five bits 0x1f: 00011111
        int8_t function = (instruction & 0x3F) //0x3f looks like so: 00111111
        //hashtable with function pointers
        instructionList[0]= &ShiftRightArithmetic;
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
                instructionList[function](reg_b, reg_c, shift); //for shifts
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
        }

    }
    else if(opcode ==  56||opcode == 50||opcode == 46||opcode == 37||opcode == 28||opcode == 23||opcode == 16|| opcode == 9||opcode == 0){
       //r type instruction
        reg_a  = (instruction >> (21) & 0x1F); //get next five bits
        reg_b = (instruction >> (16) & 0x1F); //get next five bits
        reg_c = (instruction >> (11) & 0x1F) //get next five bits
        int16_t immediate = (instruction & 0xFFFF); //get next 16 bits 0xFFF: 00001111111111111111
        
        //hashtable with function pointers
        instructionList[0]= &storeWord;
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
            if(opcode == 23 || opcode == 50){
                instructionList[opcode](immediate); //for jump
            }
            else{
                instructionList[opcode](reg_a, reg_b, immediate); // Call the function pointer
            }
        } else {
            std::cout << "Function not found for choice " << opcode << std::endl;
        }
    }
    PC +=4;
    
    //todo: separate all of the bits depending on the opcode
    //if opcode == 4 then i type, and seperate bits based on that
    //if opcode == 56, 50, 46, 37, 28, 23, 16,9, or 0 r type
    //at the end always the PC by 4
    //this code is not dry at all but i cant really think of a way to make it better
}

//rtype instructions:
void ShiftRightArithmetic(int16_t reg_b, int16_t reg_c, int8_t shift){
    registerFile[reg_c] =  (int16_t)(((uint16_t)registerFile[reg_b]) >> shift); // Sign extend after the logical shift
}

void ShiftRightLogical(int16_t reg_b, int16_t reg_c, int8_t shift){
    registerFile[reg_c] =  registerFile[reg_b] >> shift; //must be unsigned
}

void ShiftLeftLogical(int16_t reg_b, int16_t reg_c, int8_t shift){
    registerFile[reg_c] = registerFile[reg_b] << shift;
}

void Subtract(int16_t reg_a, int16_t reg_b, int16_t reg_c){
    registerFile[reg_c] = registerFile[reg_a] - registerFile[reg_b];
}

void Add(int16_t reg_a, int16_t reg_b, int16_t reg_c){
    registerFile[reg_c] = registerFile[reg_a] + registerFile[reg_b];
}

void SetLessThan(int16_t reg_a, int16_t reg_b, int16_t reg_c){
    registerFile[reg_c] = (registerFile[reg_a] < registerFile[reg_b]);
}

void Or_(int16_t reg_a, int16_t reg_b, int16_t reg_c){
    registerFile[reg_c] = registerFile[reg_a] | registerFile[reg_b];
}

void Nor_(int16_t reg_a, int16_t reg_b, int16_t reg_c){
    registerFile[reg_c] = ~(registerFile[reg_a] | registerFile[reg_b]);
}

void And_(int16_t reg_a, int16_t reg_b, int16_t reg_c){
    registerFile[reg_c] = registerFile[reg_a] & registerFile[reg_b];
}

void JumpRegister(int16_t reg_a){
    PC+= registerFile[reg_a];
}
//
//itype
//
void storeWord(int16_t reg_b, int16_t reg_c, int16_t immediate){
    int8_t byte1 = (reg_b >> 8 & 0xFF); 
    int8_t byte2 = reg_b & 0xFF;
    os.memory.setByte(registerFile[reg_c]+(immediate/8), byte1);
    os.memory.setByte(registerFile[reg_c]+(immediate/8)+1, byte2);
}

void addImm(int16_t reg_a, int16_t reg_b, int16_t immediate){
    int8_t imm_8 = immediate & 0xFF;
    registerFile[reg_b] =  registerFile[reg_a] + imm_8;
}

void LoadByteUnsigned(int16_t reg_a, int16_t reg_b, int16_t immediate){
    int8_t imm_8 = immediate & 0xFF;
    registerFile[reg_b] = os.memory.getByte(registerFile[reg_a]+imm_8);
}

void Jump(int16_t immediate){
    PC+= 4+immediate/8; 
}

void BranchNotEqual(int16_t reg_a, int16_t reg_b, int16_t immediate){
   if(registerFile[reg_a]!=registerFile[reg_b]){
        PC += 4*(immediate/8); //PC is incremented after the instruction anyways
   }
}

void BranchEqual(int16_t reg_a, int16_t reg_b, int16_t reg_c){
    if(registerFile[reg_a]==registerFile[reg_b]){
        PC += 4*(immediate/8); //PC is incremented after the instruction anyways
   }
}

void StoreByte(int16_t reg_a, int16_t reg_b, int16_t immediate){ 
    int8_t byte = reg_b & 0xFF;
    os.memory.setByte(registerFile[reg_c]+(immediate/8), byte);
}

void JumpAndLink(int16_t immediate){
    registerFile[31] = PC + 4;
    PC += 4* (immediate/8); //immediate is in bits
}

void LoadWord(int16_t reg_a, int16_t reg_b, int16_t immediate){
    uint16_t low_byte = os.memory.getByte(registerFile[reg_a]+immediate/8); // Get the low byte
    uint16_t high_byte = os.memory.getByte((registerFile[reg_a]+immediate/8)+1); // Get the high byte
    registerFile[reg_b] = (high_byte << 8) | low_byte; // Combine bytes into a word
}
