#pragma once

#include "../hardware/CPU/CPU.h"
#include "../hardware/memory.h"
#include <string>
#include <memory>

class OS {
    public:
    OS();
    
    // accessable to user
    void startup(std::string ROM_file); 
    void shutDown(); // not sure if needed
    
    // hardware
    CPU cpu;
    Memory memory;
    
    // ROM contents (might remove if we can get it in memory)
    std::unique_ptr<char[]> rom_contents_; // maybe remove later if storing ROM in memory
    
    // values
    uint32_t address_to_setup;
    uint32_t address_to_loop;
    uint32_t load_data_address;
    uint32_t program_data_address;
    uint32_t data_size;

    // memory accessors (? idk if needed in os)
    uint32_t readInt32(const std::unique_ptr<char[]> &addressSpace, const size_t& address) const;
    uint16_t readInt16(const std::unique_ptr<char[]> &addressSpace, const size_t& address) const;
    uint8_t readInt8(const std::unique_ptr<char[]> &addressSpace, const size_t& address) const;

    private:


    // file info
    std::string filename_;
    size_t file_size_;


    // official internal functions
    void resetSequence();
    void setup();
    void loop();
};