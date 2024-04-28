#pragma once

#include <vector>
#include <string>
#include <memory>

class Memory {
private:
    std::unique_ptr<unsigned int[]> contents_;
    unsigned int bufSize;
public:
    Memory();
    const unsigned int* getData() const;
    std::size_t getSize() const;
	uint32_t ReadBigEndianInt32(const size_t& addr) const;
	void clear();
};
