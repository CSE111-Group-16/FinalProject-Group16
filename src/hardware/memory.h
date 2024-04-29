#pragma once

//#include <vector>
#include <string>
#include <memory>

class OS;
class Memory {
private:
    // address spaces
    const size_t ram_size_ = 0x7000;
    const size_t io_size_ = 0x1000;
    const size_t rom_size_ = 0x8000;
    std::array<uint8_t,0x7000> address_space_ram_;
    std::array<uint8_t,0x1000> address_space_io_; // not sure if right
    std::array<uint8_t,0x8000> address_space_rom_;
    
    // helper functions
    size_t adjustAddress(size_t address) const;
public:
    Memory(OS* set_os) : os(set_os) { }
    OS* os;
    uint8_t* getByte(size_t address);
    void setByte(size_t address, uint8_t byte);
    uint8_t readByte(size_t address) const;
    const unsigned int* getData() const;
    std::size_t getSize() const;
	void clearRAM();
    void loadROM(char* contents, size_t size);
    std::string to_string();
};
