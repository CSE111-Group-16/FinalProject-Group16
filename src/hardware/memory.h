#pragma once

//#include <vector>
#include <string>
#include <memory>
#include <array>

#define SLUG_ROM_SIZE_ 0x8000
#define SLUG_RAM_SIZE_ 0x7000
#define SLUG_IO_SIZE_ 0x1000

class Console;
class Memory {
private:
    // address spaces
    const size_t ram_size_ = SLUG_RAM_SIZE_;
    const size_t io_size_ = SLUG_IO_SIZE_;
    const size_t rom_size_ = SLUG_ROM_SIZE_;
    std::array<uint8_t,SLUG_RAM_SIZE_> address_space_ram_;
    std::array<uint8_t,SLUG_IO_SIZE_> address_space_io_;
    std::array<uint8_t,SLUG_ROM_SIZE_> address_space_rom_;
    
    // helper functions
    size_t adjustAddress(size_t address) const;
public:
    Memory(Console* set_os) : console(set_os) { }
    Console* console;
    uint8_t* getByte(size_t address);
    void setByte(size_t address, uint8_t byte);
    uint8_t readByte(size_t address) const;
	void clearRAM();
    void loadROM(char* contents, size_t size);
};
