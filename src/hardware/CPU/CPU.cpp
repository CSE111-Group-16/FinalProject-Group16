#include "CPU.h"
#include "../../processes/os.h"

void CPU::resetStackPointer() {
    registerFile[29].setAddress(0x3400);
}

void CPU::PerformInstruction(const uint32_t instruction){
    
    reg_a_, reg_b_, reg_c_, shift_value_, immediate_value_ = 0;
    uint32_t opcode = (instruction >> 26);// & 0x3F);
    assert(("opcode larger than it should be", opcode < 64)); // if opcode > 63 opcode didnt get read correctly

    reg_a_  = (instruction >> (21) & 0x1F); //get next five bits
    reg_b_ = (instruction >> (16) & 0x1F); //get next five bits
    immediate_value_ = (instruction & 0xFFFF); //get next 16 bits 0xFFFF: 00001111111111111111

    if(opcode == 4){
        //r type instruction
        reg_c_ = (immediate_value_ >> 11); //get next five bits
        shift_value_ = ((immediate_value_ >> 6) & 0x1F); //get next five bits 0x1f: 00011111
        function_value_ = (immediate_value_ & 0x3F); //0x3f looks like so: 00111111

        if (r_type_instructions_.find(function_value_) != r_type_instructions_.end()) {
            (this->*r_type_instructions_[function_value_])();
        } else {
            std::cout << "Function not found for choice " << function_value_ << std::endl; //for debug
        }
    }
    else if(opcode ==  56||opcode == 50||opcode == 46||opcode == 37||opcode == 28||opcode == 23||opcode == 16|| opcode == 9||opcode == 0){
        if (i_type_instructions_.find(opcode) != i_type_instructions_.end()) {
            // call opcode function
            (this->*i_type_instructions_[opcode])();
        } else {
            std::cout << "missed" << std::endl;
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


void CPU::initialJAL(uint32_t address_to_setup) {
    PC = (address_to_setup);
    registerFile[31].address = PC+4;
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
    (*os).memory.setByte(registerFile[reg_a_].getAddress()+(immediate_value_/8), byte1);
    (*os).memory.setByte(registerFile[reg_a_].getAddress()+(immediate_value_/8)+1, byte2);
}

void CPU::addImm(){
    int16_t imm_8 = immediate_value_; // issue was here. supposed to be int16_t. add not limited to a byte.
    registerFile[reg_b_].address =  registerFile[reg_a_].getAddress() + immediate_value_;
}

void CPU::LoadByteUnsigned(){
    int8_t imm_8 = immediate_value_ & 0xFF;
    registerFile[reg_b_].address = *(*os).memory.getByte(registerFile[reg_a_].getAddress()+imm_8);
}

void CPU::Jump(){
    PC+= 4+immediate_value_; // removed /8
}

void CPU::BranchNotEqual(){
   if(registerFile[reg_a_].getAddress()!=registerFile[reg_b_].getAddress()){
        PC += 4*(immediate_value_); //PC is incremented after the instruction anyways
        // removed /8
   }
}

void CPU::BranchEqual(){
    if(registerFile[reg_a_].getAddress()==registerFile[reg_b_].getAddress()){
        PC += 4*(immediate_value_); //PC is incremented after the instruction anyways
        // removed /8
   }
}

void CPU::StoreByte(){ 
    int8_t byte = registerFile[reg_b_].getAddress() & 0xFF;
    size_t address = registerFile[reg_a_].getAddress()+(immediate_value_);
    // /std::cout<<"address: " << address << std::endl;
    (*os).memory.setByte(address, byte);
    if (address == 0x7110) {
        //std::cout<<"WRITE\n";
        std::cout << byte;
    }
}

void CPU::JumpAndLink(){
    registerFile[31].address = PC + 4;
    PC += 4* (immediate_value_); //immediate is in bits
    // removed /8
}

void CPU::LoadWord(){
    // removed /8 from both immediate_value
    uint16_t low_byte = *(*os).memory.getByte(registerFile[reg_a_].getAddress()+immediate_value_); // Get the low byte
    uint16_t high_byte = *(*os).memory.getByte((registerFile[reg_a_].getAddress()+immediate_value_)+1); // Get the high byte
    registerFile[reg_b_].address = (high_byte << 8) | low_byte; // Combine bytes into a word
}
