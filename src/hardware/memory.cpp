#include "memory.h"
#include "../processes/os.h"
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <array>

/// @brief returns a POINTER to the byte stored at address
/// @param address refers to the FULL address, not adjusted
/// @return 
uint8_t* Memory::getByte(size_t address) {
    // address is in RAM address space
    if (address < ram_size_) {
        return &address_space_ram_[adjustAddress(address)];
    }
    // address is in IO address space
    else if (address < rom_size_) {
        return &address_space_io_[adjustAddress(address)];
    }
    // address is in ROM address space
    else if (address < 2*rom_size_) {
        return &address_space_rom_[adjustAddress(address)];
    }
	else {
        throw std::out_of_range("address out of range");
    }
}

/// @brief sets the byte at address to 
/// @param address refers to the FULL address, not adjusted
/// @param byte uint8_t to set
void Memory::setByte(size_t address, uint8_t byte) {
    // address is in RAM address space
    if (address < ram_size_) {
        uint8_t* value = &address_space_ram_[adjustAddress(address)];
        *value = byte; // should set the value correctly
        assert(readByte(address) == byte);
    }
    // address is in IO address space
    else if (address < rom_size_) {
        uint8_t* value = &address_space_io_[adjustAddress(address)];
        *value = byte; // should set the value correctly
        assert(readByte(address) == byte);
    }
    // address is in ROM address space
    else if (address < 2*rom_size_) {
        uint8_t* value = &address_space_rom_[adjustAddress(address)];
        *value = byte; // should set the value correctly
        assert(readByte(address) == byte);
    }
	else {
        throw std::out_of_range("address out of range");
    }
}

/// @brief returns the byte VALUE at address in memory
/// @param address refers to the FULL address, not adjusted
uint8_t Memory::readByte(size_t address) const {
    //std::cout << address << std::endl;
    // address is in RAM address space
    if (address < ram_size_) {
        return address_space_ram_[adjustAddress(address)];
    }
    // address is in IO address space
    else if (address < rom_size_) {
        return address_space_io_[adjustAddress(address)];
    }
    // address is in ROM address space
    else if (address < 0x10000) {
        return address_space_rom_[adjustAddress(address)];
    }
	else {
        throw std::out_of_range("address out of range");
    }
}

/// @brief adjusts full address as needed
/// @param address 
/// @return 
size_t Memory::adjustAddress(size_t address) const {
    // address is in RAM address space
    if (address < ram_size_) {
        return address;
    }
    // address is in IO address space
    else if (address < rom_size_) {
        return address - ram_size_;
    }
    // address is in ROM address space
    else if (address < 2*rom_size_) {
        return address - rom_size_;
    }
	else {
        std::cout << address << std::endl;
        throw std::out_of_range("address out of range");
    }
}

void Memory::clearRAM() {
    for (size_t i=0; i<ram_size_; i++) {
        address_space_ram_[i] = 0;
    }
}

void Memory::loadROM(char* contents, size_t size) {
    size_t offset = SLUG_ROM_SIZE_;
    for (size_t i=0; i<size; i++) {
        setByte(offset+i, (uint8_t)contents[i]);
    }
}