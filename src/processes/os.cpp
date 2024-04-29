#include "os.h"

#include <fstream>
#include <iostream>

OS::OS() {
    cpu = CPU(this);
    memory = Memory(this);
}

void OS::resetSequence() {
    // clearing RAM with zeros;
    memory.clear();
    
    // opening ROM file
    std::ifstream file(filename_);

    // check if file opened successfully
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename_ << std::endl;
        return;
    }

    // saving file size
    file_size_ = file.tellg();

    // set file pointer to beginning
    file.seekg(0, std::ios::beg);

    rom_contents_ = std::make_unique<char[]>(file_size_);

    // read rom into rom_contents_ and memory (if possible)
    file.read(rom_contents_.get(), file_size_);

    // close slug file
    file.close();
    
    // load ROM into RAM
    memory.loadROM(rom_contents_); // put ROM into memory (at address 0x8000);
    
    // get relevant info from ROM
    address_to_setup = readInt32(rom_contents_, 0x01e0);
    address_to_loop = readInt32(rom_contents_, 0x01e4);
    load_data_address = readInt32(rom_contents_, 0x01e8);
    program_data_address = readInt32(rom_contents_, 0x01ec);
    data_size = readInt32(rom_contents_, 0x01f0);
    
    // copy data to RAM (TODO)
    memory.loadData();


    // set stack pointer reg to end of stack
    
}


uint32_t OS::readInt32(const std::unique_ptr<char[]> &addressSpace, const size_t& address) const {
    uint32_t out = 0;
    for (int i = 0; i < 4; i++) {
        out <<= 8;
        out |= (uint8_t)addressSpace[address + i];
    }
    return out;
}

uint16_t OS::readInt16(const std::unique_ptr<char[]> &addressSpace, const size_t& address) const {
    uint32_t out = 0;
    for (int i = 0; i < 2; i++) {
        out <<= 8;
        out |= (uint8_t)addressSpace[address + i];
    }
    return out;
}

uint8_t OS::readInt8(const std::unique_ptr<char[]> &addressSpace, const size_t& address) const {
    return (uint8_t)addressSpace[address];;
}