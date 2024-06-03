#include "CPU.h"
#include "../../processes/console.h"
#include <bitset>

void CPU::resetStackPointer() {
    registerFile[register_sp].setValue(top_of_stack);
}

void CPU::PerformInstruction(const uint32_t instruction){
    reg_a_, reg_b_, reg_c_, shift_value_, immediate_value_ = 0;
    uint32_t opcode = (instruction >> opcode_shift);
    assert(("opcode larger than it should be ", opcode < opcode_max)); // if opcode > 63 opcode didnt get read correctly

    reg_a_  = (instruction >> (reg_a_shift) & five_bitmask); //get next five bits
    reg_b_ = (instruction >> (reg_b_shift) & five_bitmask); //get next five bits
    immediate_value_ = (instruction & sixteen_bitmask); //get next 16 bits 0xFFFF: 00001111111111111111

    if(opcode == rtype){
        //r-type instruction
        reg_c_ = (immediate_value_ >> reg_c_shift); //get next five bits
        shift_value_ = ((immediate_value_ >> shift_shift) & five_bitmask); //get next five bits 0x1f: 00011111
        function_value_ = (immediate_value_ & six_bitmask); //0x3f: 00111111        

        if (logInstructionBreakdown) r_typeBreakdown(opcode);

        // run instruction
        if (r_type_instructions_.find(function_value_) != r_type_instructions_.end()) {
            (this->*r_type_instructions_[function_value_])();
        } else {
            std::cerr << "Function not found for choice " << function_value_ << std::endl; //for debug
        }
    }
    else if(opcode ==  LW||opcode == JAL||opcode == SB||opcode == BEQ||opcode == BNE||opcode == J||opcode == LBU|| opcode == ADDI||opcode == SW){
        if (i_type_instructions_.find(opcode) != i_type_instructions_.end()) {
            // i-type instructions
            if (logInstructionBreakdown) i_typeBreakdown(opcode);

            // run instruction
            (this->*i_type_instructions_[opcode])();
        } else {
            std::cerr << "missed" << std::endl;
            std::cerr << "Function not found for choice " << opcode << std::endl;
        }
    }
    else{
        PC +=4;
    }
    if (logFullRegisters) logAllRegisters();
}

void CPU::initialJAL(uint32_t address_to_jump) {
    registerFile[register_ra].setValue(PC+4);
    PC = (address_to_jump);
}

//rtype instructions:
void CPU::ShiftRightArithmetic(){
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(false, true, true, false);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;
    if (logInstructionOperation) {
        (*(*console).getLogger()) << "\tSRA: R[reg_c] = (signed) R[reg_b] >> shift_value" << std::endl;
        (*(*console).getLogger()) << "\tR[" << registerFile[reg_c_].getName() << "] = (signed) R[" << registerFile[reg_b_].getName() << "] >> " << shift_value_ <<  std::endl;
        (*(*console).getLogger()) << "\t" << registerFile[reg_c_].getValue() << " = " << registerFile[reg_b_].getValue() << " >> " << shift_value_ <<  std::endl;
        (*(*console).getLogger()) << "\tR[" << registerFile[reg_c_].getName() << "]: " << registerFile[reg_c_].getValue() << " => ";
    }

    // operation
    registerFile[reg_c_].setValue((int16_t)((registerFile[reg_b_].getValue()) >> shift_value_)); // Sign extend after the logical shift
    
    if (logInstructionOperation) (*(*console).getLogger()) << registerFile[reg_c_].getValue() << std::endl;
    if (logPostInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(false, true, true, false);
    }
    PC +=4;
}

void CPU::ShiftRightLogical(){
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(false, true, true, false);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;

    // operation
    registerFile[reg_c_].setValue(registerFile[reg_b_].getValue() >> shift_value_); //must be unsigned
    
    if (logPostInstructionReg) logRegisters(false, true, true, false);
    PC +=4;
}

void CPU::ShiftLeftLogical(){
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(false, true, true, false);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;

    // operation
    registerFile[reg_c_].setValue(registerFile[reg_b_].getValue() << shift_value_);
    
    if (logPostInstructionReg) logRegisters(true, true, true, false);
    PC +=4;
}

void CPU::Subtract(){
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(true, true, true, false);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;
    // operation
    registerFile[reg_c_].setValue(registerFile[reg_a_].getValue() - registerFile[reg_b_].getValue());
    
    if (logPostInstructionReg) logRegisters(true, true, true, false);
    PC +=4;
}

void CPU::Add(){
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(false, true, true, false);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;

    // operation
    int16_t add = registerFile[reg_a_].getValue() + registerFile[reg_b_].getValue();
    registerFile[reg_c_].setValue(add);
    
    if (logPostInstructionReg) logRegisters(true, true, true, false);
    PC +=4;

}

void CPU::SetLessThan(){
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(true, true, true, false);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;

    // operation
    registerFile[reg_c_].setValue(registerFile[reg_a_].getValue() < registerFile[reg_b_].getValue());
    
    if (logPostInstructionReg) logRegisters(true, true, true, false);
    PC +=4;
}

void CPU::Or_(){
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(true, true, true, false);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;

    // operation
    registerFile[reg_c_].setValue(registerFile[reg_a_].getValue() | registerFile[reg_b_].getValue());
    
    if (logPostInstructionReg) logRegisters(true, true, true, false);
    PC +=4;
}

void CPU::Nor_(){
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(true, true, true, false);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;
    
    // operation
    registerFile[reg_c_].setValue(~(registerFile[reg_a_].getValue() | registerFile[reg_b_].getValue()));
    
    if (logPostInstructionReg) logRegisters(true, true, true, false);
    PC +=4;
}

void CPU::And_(){
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(true, true, true, false);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;

    // operation
    registerFile[reg_c_].setValue(registerFile[reg_a_].getValue() & registerFile[reg_b_].getValue());
    
    if (logPostInstructionReg) logRegisters(true, true, true, false);
    PC +=4;
}

void CPU::JumpRegister(){
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(true, false, false, true);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;

    // operation
    PC= registerFile[reg_a_].getValue();
    if (logPostInstructionReg) logRegisters(true, false, false, true);
}

//
// itype instructions
//
void CPU::storeWord(){
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(true, true, true, false);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;

    // operation
    uint8_t byte1 = ((registerFile[reg_b_].getValue() >> byte_shift) & eight_bitmask); 
    uint8_t byte2 = registerFile[reg_b_].getValue() & eight_bitmask;
    (*(*console).getMemory()).setByte(registerFile[reg_a_].getValue()+(immediate_value_), byte1); //byte_shift
    (*(*console).getMemory()).setByte(registerFile[reg_a_].getValue()+(immediate_value_)+1, byte2);//byte_shift removed
    
    if (registerFile[reg_a_].getValue()+(immediate_value_) == STDOUT_DATA_) {
        (*(*console).getLogger()) << "WROTE FROM storeWord (?)" << std::endl;
        std::cout << byte1 << byte2;
    }
    
    if (logPostInstructionReg) logRegisters(true, true, true, false);
    PC +=4;
}

void CPU::addImm(){
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(true, true, true, false);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;

    registerFile[reg_b_].setValue(registerFile[reg_a_].getValue() + immediate_value_);
    
    if (logPostInstructionReg) logRegisters(true, true, true, false);
    PC +=4;

}

void CPU::LoadByteUnsigned(){
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(true, true, true, false);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;

    if (registerFile[reg_a_].getValue()+immediate_value_ == STDIN_DATA_) {
        // load from stdin
        uint8_t byte;
        std::cin >> byte;
        if (logStdin) (*(*console).getLogger()) << "read byte from stdin: " << byte <<std::endl;
        registerFile[reg_b_].setValue(byte);
    } else if (registerFile[reg_a_].getValue()+immediate_value_ == CONTROLLER_DATA_) {
        // load input from controller
        uint8_t byte = (*console).getControllerByte() & eight_bitmask;
        registerFile[reg_b_].setValue(byte);
    } else {
        // load from memory
        registerFile[reg_b_].setValue((*(*console).getMemory()).readByte(registerFile[reg_a_].getValue()+immediate_value_));
    }
    if (logPostInstructionReg) logRegisters(true, true, true, false);
    PC +=4;
}

void CPU::Jump(){
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(true, true, true, true);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;

    PC+= 4*immediate_value_; // removed /8, also changed + to *
    if (logPostInstructionReg) logRegisters(true, true, true, true);
}

void CPU::BranchNotEqual(){
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(true, true, true, true);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;

    
   if(registerFile[reg_a_].getValue()!=registerFile[reg_b_].getValue()){
        PC += 4+4*(immediate_value_); //PC is incremented after the instruction anyways
        if (logStderr) (*(*console).getLogger()) << "not equal" << std::endl;
        if (logPostInstructionReg) logRegisters(true, true, true, true);
   }
   else{
        if (logStderr) (*(*console).getLogger()) << "equal" << std::endl;
        if (logPostInstructionReg) logRegisters(true, true, true, true);
        PC+=4;
   }
}

void CPU::BranchEqual(){
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(true, true, true, true);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;

    if(registerFile[reg_a_].getValue()==registerFile[reg_b_].getValue()){
        PC += 4+4*(immediate_value_); 
        if (logPostInstructionReg) logRegisters(true, true, true, false);
        //(*(*console).getLogger()) << "equal" << std::endl;
   }
   else{
        if (logPostInstructionReg) logRegisters(true, true, true, false);
        PC+=4;
   }
}

void CPU::StoreByte(){ 
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(true, true, true, false);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;

    int8_t byte = registerFile[reg_b_].getValue() & eight_bitmask;
    size_t value = registerFile[reg_a_].getValue()+(immediate_value_);
    (*(*console).getMemory()).setByte(value, byte);
    
    // write to stdout
    if (value == STDOUT_DATA_) {
        std::cout << byte;
        if (logStdout) (*(*console).getLogger()) << "stdout: " << byte << std::endl;
    }
    // write to stderr
    if (value == STDERR_DATA_) {
        std::cerr << byte;
        if (logStderr) (*(*console).getLogger()) << "stderr: " << byte << std::endl;
    }
    // exit condition
    if (value == STOP_EXECUTION_) {
        (*(*console).getLogger()) << "\n\nEnd Condition Triggered!" << std::endl;
        (*console).setExitCondition(true);
    }

    if (logPostInstructionReg) logRegisters(true, true, true, false);
    PC +=4;
}

void CPU::JumpAndLink(){
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(true, true, true, true);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;

    registerFile[register_ra].setValue(PC + 4);
    PC = 4* (immediate_value_);

    if (logPostInstructionReg) logRegisters(true, true, true, true);
}

void CPU::LoadWord(){
    if (logPreInstructionReg) {
        (*(*console).getLogger()) << "Pre instruction: " << std::endl;
        logRegisters(true, true, true, false);
    }
    if (logInstructionName) (*(*console).getLogger()) << __func__ << ":" << std::endl;
    if(registerFile[reg_b_].getName() == "x0"){
        registerFile[reg_b_].setValue(0);
        return;
    }
    uint16_t high_byte = *(*(*console).getMemory()).getByte(registerFile[reg_a_].getValue()+immediate_value_); // Get the low byte
    uint16_t low_byte = *(*(*console).getMemory()).getByte((registerFile[reg_a_].getValue()+immediate_value_)+1); // Get the high byte
    registerFile[reg_b_].setValue((high_byte << byte_shift) | low_byte); // Combine bytes into a word
    
    if (logPostInstructionReg) logRegisters(true, true, true, false);
    PC +=4;
}

/// @brief Allows for selective logging of register values. Parameters are toggles
/// @param reg_a
/// @param reg_b 
/// @param reg_c 
/// @param reg_pc 
void CPU::logRegisters(bool reg_a, bool reg_b, bool reg_c, bool reg_pc) {
    if (reg_a)  (*(*console).getLogger()) << "  reg a: " << registerFile[reg_a_].getName() << " = " << registerFile[reg_a_].getValue() << std::endl;
    if (reg_b)  (*(*console).getLogger()) << "  reg b: " << registerFile[reg_b_].getName() << " = " << registerFile[reg_b_].getValue() << std::endl;
    if (reg_c)  (*(*console).getLogger()) << "  reg c: " << registerFile[reg_c_].getName() << " = " << registerFile[reg_c_].getValue() << std::endl;
    if (reg_pc) (*(*console).getLogger()) << "  PC at: " << PC << std::endl;
}

/// @brief Logs ALL register values
void CPU::logAllRegisters() {
    (*(*console).getLogger()) << "All Registers:" << std::endl;
    for (int i = 0; i < 32; i+=4) {
        (*(*console).getLogger()) << std::setw(2) << registerFile[i].getName()   << "(" << std::setw(2) << i  << "): "<< std::setw(4) << registerFile[i].getValue() << ", ";
        (*(*console).getLogger()) << std::setw(2) << registerFile[i+1].getName() << "(" << std::setw(2) << i+1<<"): " << std::setw(4) << registerFile[i+1].getValue() << ", ";
        (*(*console).getLogger()) << std::setw(2) << registerFile[i+2].getName() << "(" << std::setw(2) << i+2<<"): " << std::setw(4) << registerFile[i+2].getValue() << ", ";
        (*(*console).getLogger()) << std::setw(2) << registerFile[i+3].getName() << "(" << std::setw(2) << i+3<<"): " << std::setw(4) << registerFile[i+3].getValue() << std::endl;
    }
}

void CPU::i_typeBreakdown(uint32_t opcode) {
    (*(*console).getLogger()) << "Instruction breakdown:" << std::endl;
    (*(*console).getLogger()) << "  Opcode   : " << opcode << std::endl;
    (*(*console).getLogger()) << "  reg_a    : " << reg_a_ << std::endl;
    (*(*console).getLogger()) << "  reg_b    : " << reg_b_ << std::endl;
    (*(*console).getLogger()) << "  immediate: " << immediate_value_ << std::endl;
    
}

void CPU::r_typeBreakdown(uint32_t opcode) {
    (*(*console).getLogger()) << "Instruction breakdown:" << std::endl;
    (*(*console).getLogger()) << "  Opcode: " << opcode << std::endl;
    (*(*console).getLogger()) << "  reg_a : " << reg_a_ << std::endl;
    (*(*console).getLogger()) << "  reg_b : " << reg_b_ << std::endl;
    (*(*console).getLogger()) << "  reg_c : " << reg_c_ << std::endl;
    (*(*console).getLogger()) << "  shift : " << shift_value_ << std::endl;
    (*(*console).getLogger()) << "  func  : " << function_value_ << std::endl;
}


uint8_t CPU::readController() {
    uint8_t byte = 0x00;
    if ((*console).getEventHandler().type == SDL_KEYDOWN) {
        //Select surfaces based on key press
        switch( (*console).getEventHandler().key.keysym.sym )
        {
            case SDLK_UP:
                byte = byte | CONTROLLER_UP_MASK;
                break;
            case SDLK_DOWN:
                byte = byte | CONTROLLER_DOWN_MASK;
                break;
            case SDLK_LEFT:
                byte = byte | CONTROLLER_LEFT_MASK;
                break;
            case SDLK_RIGHT:
                byte = byte | CONTROLLER_RIGHT_MASK;
                break;
        }
    }
	else if((*console).getEventHandler().type == SDL_KEYUP)
	{
		switch( (*console).getEventHandler().key.keysym.sym )
		{
			case SDLK_UP:
				byte = byte & ~CONTROLLER_UP_MASK;
				break;
			case SDLK_DOWN:
				byte = byte & ~CONTROLLER_DOWN_MASK;
				break;
			case SDLK_LEFT:
				byte = byte & ~CONTROLLER_LEFT_MASK;
				break;
			case SDLK_RIGHT:
				byte = byte & ~CONTROLLER_RIGHT_MASK;
				break;
		}

	}
    return byte;
}