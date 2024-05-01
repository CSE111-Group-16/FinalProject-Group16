#include "CPU.h"
#include "../../processes/os.h"

// CPU::CPU() : PC(0x400, "PC") {
//     registerFile = new register[32];
//     SetUpRegisters(); // Initialize register names and addresses
// }

// CPU::CPU(register* registerFile, register PC) : PC(PC), registerFile(registerFile) {
//     SetUpRegisters(); // Initialize register names and addresses
// }

// CPU::~CPU() {
//     delete[] registerFile; // Free the dynamically allocated registerFile array
// }

// void CPU::SetUpRegisters(){
//     std::string names[32] = {
//         "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
//         "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
//         "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
//         "s8", "s9", "k0", "k1", "gp", "sp", "fp", "ra"
//     };
//     //initialize all of the register names
//     for (int i = 0; i < 32; i++) {
//         registerFile[i] = Register(0x00, names[i]);
//     }
//     // registerFile[29].setAddress(0x3400); //29 is the stack pointer
// }

//todo:
//one function that takes in an instruction
//and determines the registers it needs, the instruction 
//based on the opcode
void CPU::resetStackPointer() {
    registerFile[29].setAddress(0x3400);
}

void CPU::PerformInstruction(const uint32_t instruction){
    
    reg_a_, reg_b_, reg_c_ = 0;
    uint8_t opcode = (instruction >> (25) & 0x3F);
    assert(("opcode larger than it should be", opcode < 64)); // if opcode > 63 opcode didnt get read correctly

    if(opcode == 4){
        //r type instruction
        reg_a_  = (instruction >> (21) & 0x1F); //get next five bits
        reg_b_ = (instruction >> (16) & 0x1F); //get next five bits
        reg_c_ = (instruction >> (11) & 0x1F); //get next five bits
        shift_value_ = (instruction >> (6) & 0x1F); //get next five bits 0x1f: 00011111
        function_value_ = (instruction & 0x3F); //0x3f looks like so: 00111111

        if (r_type_instructions_.find(function_value_) != r_type_instructions_.end()) {
            (this->*r_type_instructions_[function_value_])();
            //r_type_instructions_[function_value_]();

            // if(function == 0 || function == 35 || function == 40){
            //     r_type_instructions_[function](reg_b, reg_c, shift); //for shifts
            // }
            // else if(function == 33){
            //     r_type_instructions_[function](reg_a);//for jump register
            // }
            // else{
            //     r_type_instructions_[function](reg_a, reg_b, reg_c); // Call the function pointer
            // }
             
            //there may be a problem here however, in that the shift value is only used in shift and will give the warning 
            //"unused variable"
        } else {
            std::cout << "Function not found for choice " << function_value_ << std::endl; //for debug
        }

    }
    else if(opcode ==  56||opcode == 50||opcode == 46||opcode == 37||opcode == 28||opcode == 23||opcode == 16|| opcode == 9||opcode == 0){
        // i type instruction
        reg_a_  = (instruction >> (21) & 0x1F); //get next five bits
        reg_b_ = (instruction >> (16) & 0x1F); //get next five bits
        reg_c_ = (instruction >> (11) & 0x1F); //get next five bits
        immediate_value_ = (instruction & 0xFFFF); //get next 16 bits 0xFFF: 00001111111111111111
        
        //hashtable with function pointers

        if (i_type_instructions_.find(opcode) != i_type_instructions_.end()) {
            (this->*i_type_instructions_[opcode])();
            //i_type_instructions_[opcode]();
            
            // instructionList[opcode](reg_a, reg_b reg_c, immediate); // Call the function pointer
            // if(opcode == 23 || opcode == 50){
            //     instructionList[opcode](immediate); //for jump
            // }
            // else{
            //     instructionList[opcode](reg_a, reg_b, immediate); // Call the function pointer
            // }
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
void CPU::ShiftRightArithmetic(){
    registerFile[reg_c_].address =  (int16_t)(((uint16_t)registerFile[reg_b_].getAddress()) >> shift_value_); // Sign extend after the logical shift
}

void CPU::ShiftRightLogical(){
    registerFile[reg_c_].address =  registerFile[reg_b_].getAddress() >> shift_value_; //must be unsigned
}

void CPU::ShiftLeftLogical(){
    registerFile[reg_c_].address = registerFile[reg_b_].getAddress() << shift_value_;
}

void CPU::Subtract(){
    registerFile[reg_c_].address = registerFile[reg_a_].getAddress() - registerFile[reg_b_].getAddress();
}

void CPU::Add(){
    registerFile[reg_c_].address = registerFile[reg_a_].getAddress() + registerFile[reg_b_].getAddress();
}

void CPU::SetLessThan(){
    registerFile[reg_c_].address = (registerFile[reg_a_].getAddress() < registerFile[reg_b_].getAddress());
}

void CPU::Or_(){
    registerFile[reg_c_].address = registerFile[reg_a_].getAddress() | registerFile[reg_b_].getAddress();
}

void CPU::Nor_(){
    registerFile[reg_c_].address = ~(registerFile[reg_a_].getAddress() | registerFile[reg_b_].getAddress());
}

void CPU::And_(){
    registerFile[reg_c_].address = registerFile[reg_a_].getAddress() & registerFile[reg_b_].getAddress();
}

void CPU::JumpRegister(){
    PC+= registerFile[reg_a_].getAddress();
}
//
//itype
//
void CPU::storeWord(){
    int8_t byte1 = (reg_b_ >> 8 & 0xFF); 
    int8_t byte2 = reg_b_ & 0xFF;
    (*os).memory.setByte(registerFile[reg_c_].getAddress()+(immediate_value_/8), byte1);
    (*os).memory.setByte(registerFile[reg_c_].getAddress()+(immediate_value_/8)+1, byte2);
}

void CPU::addImm(){
    int8_t imm_8 = immediate_value_ & 0xFF;
    registerFile[reg_b_].address =  registerFile[reg_a_].getAddress() + imm_8;
}

void CPU::LoadByteUnsigned(){
    int8_t imm_8 = immediate_value_ & 0xFF;
    registerFile[reg_b_].address = *(*os).memory.getByte(registerFile[reg_a_].getAddress()+imm_8);
}

void CPU::Jump(){
    PC+= 4+immediate_value_/8; 
}

void CPU::BranchNotEqual(){
   if(registerFile[reg_a_].getAddress()!=registerFile[reg_b_].getAddress()){
        PC += 4*(immediate_value_/8); //PC is incremented after the instruction anyways
   }
}

void CPU::BranchEqual(){
    if(registerFile[reg_a_].getAddress()==registerFile[reg_b_].getAddress()){
        PC += 4*(immediate_value_/8); //PC is incremented after the instruction anyways
   }
}

void CPU::StoreByte(){ 
    int8_t byte = reg_b_ & 0xFF;
    (*os).memory.setByte(registerFile[reg_c_].getAddress()+(immediate_value_/8), byte);
}

void CPU::JumpAndLink(){
    registerFile[31].address = PC + 4;
    PC += 4* (immediate_value_/8); //immediate is in bits
}

void CPU::LoadWord(){
    uint16_t low_byte = *(*os).memory.getByte(registerFile[reg_a_].getAddress()+immediate_value_/8); // Get the low byte
    uint16_t high_byte = *(*os).memory.getByte((registerFile[reg_a_].getAddress()+immediate_value_/8)+1); // Get the high byte
    registerFile[reg_b_].address = (high_byte << 8) | low_byte; // Combine bytes into a word
}
