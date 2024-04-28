#include "src/hardware/memory.h"

#include <fstream>
#include <iostream>

Memory::Memory() {
    uint8_t bufSize = 0x7000;
    contents_ = std::make_unique<uint8_t[]>(bufSize);
}

const uint8_t* Memory::getData() const {
    return contents_.get();
}

std::size_t Memory::getSize() const {
    return bufSize;
}

uint32_t Memory::ReadBigEndianInt32(const size_t& addr) const {
    uint32_t out = 0;
    for (int i = 0; i < 4; i++) {
        out <<= 8;
        out |= (uint8_t)contents_[addr + i];
    }
    return out;
}

void Memory::clear() {
	uint8_t *begin = contents_.get();
    uint8_t *end = begin + bufSize;
    std::fill(begin, end, 0);
}