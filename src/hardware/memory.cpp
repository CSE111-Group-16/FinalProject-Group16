#include "src/hardware/memory.h"

#include <iostream>

Memory::Memory() {
    std::size_t bufSize = 0x7000;
    contents_ = std::make_unique<char[]>(bufSize);
}

const int Memory::getData() const {
	// returns a pointer to contents
    return contents_.get();
}

std::size_t Memory::getSize() const {
	// returns max size of contents
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
	// using std::fill to fill contents_ with zeroes
	char *begin = contents_.get();
    char *end = begin + bufSize;
    std::fill(begin, end, 0);
}