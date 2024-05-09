#include "CPU.h"
#include "../../processes/os.h"

void CPU::resetStackPointer() {
    registerFile[register_sp].setAddress(top_of_stack);
}

void CPU::PerformInstruction(const uint32_t instruction){
    
    reg_a_, reg_b_, reg_c_, shift_value_, immediate_value_ = 0;
    uint32_t opcode = (instruction >> opcode_shift);// & 0x3F);
    assert(("opcode larger than it should be", opcode < opcode_max)); // if opcode > 63 opcode didnt get read correctly

    reg_a_  = (instruction >> (reg_a_shift) & five_bitmask); //get next five bits
    reg_b_ = (instruction >> (reg_b_shift) & five_bitmask); //get next five bits
    immediate_value_ = (instruction & sixteen_bitmask); //get next 16 bits 0xFFFF: 00001111111111111111

    if(opcode == rtype){
        //r type instruction
        reg_c_ = (immediate_value_ >> reg_c_shift); //get next five bits
        shift_value_ = ((immediate_value_ >> shift_shift) & five_bitmask); //get next five bits 0x1f: 00011111
        function_value_ = (immediate_value_ & six_bitmask); //0x3f looks like so: 00111111

        if (r_type_instructions_.find(function_value_) != r_type_instructions_.end()) {
            (this->*r_type_instructions_[function_value_])();
        } else {
            std::cerr << "Function not found for choice " << function_value_ << std::endl; //for debug
        }
    }
    else if(opcode ==  LW||opcode == JAL||opcode == SB||opcode == BEQ||opcode == BNE||opcode == J||opcode == LBU|| opcode == ADDI||opcode == SW){
        if (i_type_instructions_.find(opcode) != i_type_instructions_.end()) {
            // call opcode function
            (this->*i_type_instructions_[opcode])();
        } else {
            std::cerr << "missed" << std::endl;
            std::cerr << "Function not found for choice " << opcode << std::endl;
        }
    }
    // moving increment to individual instructions
    //PC +=4;
    
    //todo: separate all of the bits depending on the opcode
    //if opcode == 4 then i type, and seperate bits based on that
    //if opcode == 56, 50, 46, 37, 28, 23, 16,9, or 0 r type
    //at the end always the PC by 4
    //this code is not dry at all but i cant really think of a way to make it better
}


void CPU::initialJAL(uint32_t address_to_jump) {
    registerFile[register_ra].address = PC+4;
    PC = (address_to_jump);
}

//rtype instructions:
void CPU::ShiftRightArithmetic(){
    registerFile[reg_c_].address =  (int16_t)(((uint16_t)registerFile[reg_b_].getAddress()) >> shift_value_); // Sign extend after the logical shift
    PC +=4;
}

void CPU::ShiftRightLogical(){
    registerFile[reg_c_].address =  registerFile[reg_b_].getAddress() >> shift_value_; //must be unsigned
    PC +=4;
}

void CPU::ShiftLeftLogical(){
    registerFile[reg_c_].address = registerFile[reg_b_].getAddress() << shift_value_;
    PC +=4;
}

void CPU::Subtract(){
    registerFile[reg_c_].address = registerFile[reg_a_].getAddress() - registerFile[reg_b_].getAddress();
    PC +=4;
}

void CPU::Add(){
    registerFile[reg_c_].address = registerFile[reg_a_].getAddress() + registerFile[reg_b_].getAddress();
    PC +=4;
}

void CPU::SetLessThan(){
    registerFile[reg_c_].address = (registerFile[reg_a_].getAddress() < registerFile[reg_b_].getAddress());
    PC +=4;
}

void CPU::Or_(){
    registerFile[reg_c_].address = registerFile[reg_a_].getAddress() | registerFile[reg_b_].getAddress();
    PC +=4;
}

void CPU::Nor_(){
    registerFile[reg_c_].address = ~(registerFile[reg_a_].getAddress() | registerFile[reg_b_].getAddress());
    PC +=4;
}

void CPU::And_(){
    registerFile[reg_c_].address = registerFile[reg_a_].getAddress() & registerFile[reg_b_].getAddress();
    PC +=4;
}

void CPU::JumpRegister(){
    PC= registerFile[reg_a_].getAddress();
}
//
//itype
//
void CPU::storeWord(){
    int8_t byte1 = ((registerFile[reg_b_].getAddress() >> byte_shift) & eight_bitmask); 
    int8_t byte2 = registerFile[reg_b_].getAddress() & eight_bitmask;
    (*os).memory.setByte(registerFile[reg_a_].getAddress()+(immediate_value_), byte1); //byte_shift
    (*os).memory.setByte(registerFile[reg_a_].getAddress()+(immediate_value_)+1, byte2);//byte_shift removed
    if (registerFile[reg_a_].getAddress()+(immediate_value_) == 0x7110) {
        std::cerr << byte1 << byte2;
    }
    //else if(registerFile[reg_a_].getAddress()+(immediate_value_) == 0x7200){
      //  exit(EXIT_FAILURE);
    //}
    PC +=4;
}

void CPU::addImm(){
    //std::cout << "reg a before addi" << registerFile[reg_a_].getAddress() + immediate_value_ << std::endl;
    //std::cout << "reg b before addi" << registerFile[reg_b_].getAddress() << std::endl;

    registerFile[reg_b_].address =  registerFile[reg_a_].getAddress() + immediate_value_;
    PC +=4;
}

void CPU::LoadByteUnsigned(){
    //registerFile[reg_b_].address = os->readInt8(registerFile[reg_a_].getAddress()+immediate_value_);
    int8_t imm_8 = immediate_value_ & eight_bitmask;
    registerFile[reg_b_].address = *(*os).memory.getByte(registerFile[reg_a_].getAddress()+imm_8);
    PC +=4;
}

void CPU::Jump(){
    PC+= 4*immediate_value_; // removed /8, also changed + to *
}

void CPU::BranchNotEqual(){
    //std::cout << registerFile[reg_a_].getAddress() << std::endl;
    //std::cout << registerFile[reg_b_].getAddress() << std::endl;

   if(registerFile[reg_a_].getAddress()!=registerFile[reg_b_].getAddress()){
        PC += 4*(immediate_value_); //PC is incremented after the instruction anyways
        std::cout << "not equal" << std::endl;

        // removed /8
   }
}

void CPU::BranchEqual(){
    //std::cout << registerFile[reg_a_].getAddress() << std::endl;
    //std::cout << registerFile[reg_b_].getAddress() << std::endl;
    if(registerFile[reg_a_].getAddress()==registerFile[reg_b_].getAddress()){
        PC += 4*(immediate_value_); //PC is incremented after the instruction anyways
        std::cout << "equal" << std::endl;
   }
}

void CPU::StoreByte(){ 
    int8_t byte = registerFile[reg_b_].getAddress() & eight_bitmask;
    size_t address = registerFile[reg_a_].getAddress()+(immediate_value_);
    // /std::cout<<"address: " << address << std::endl;
    (*os).memory.setByte(address, byte);
    
    // write to stdout
    if (address == 0x7110) {
        //std::cout<<"WRITE\n";
        std::cerr << byte;
    }
    // write to stderr
    if (address == 0x7120) {
        //std::cout<<"WRITE\n";
        std::cerr << byte;
    }
    // exit condition
    if (address == 0x7200) {
        os->exitCondition = true;
    }
    PC +=4;
}

void CPU::JumpAndLink(){
    registerFile[register_ra].address = PC + 4;
    PC += 4* (immediate_value_); //immediate is in bits
    // removed /8
}

void CPU::LoadWord(){
    //std::cout << "register a before lw " << std::hex << registerFile[reg_a_].getAddress()+immediate_value_ << std::endl;
    //std::cout << "register b before lw " << std::hex << registerFile[reg_b_].address << std::endl;
    uint16_t low_byte = *(*os).memory.getByte(registerFile[reg_a_].getAddress()+immediate_value_); // Get the low byte
    uint16_t high_byte = *(*os).memory.getByte((registerFile[reg_a_].getAddress()+immediate_value_)+1); // Get the high byte
    registerFile[reg_b_].address = (high_byte << byte_shift) | low_byte; // Combine bytes into a word
    PC +=4;
}