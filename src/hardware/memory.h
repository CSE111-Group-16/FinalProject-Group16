#pragma once

#include <vector>
#include <string>
#include <memory>

class Memory {
private:
    std::unique_ptr<uint8_t[]> contents_;
    std::size_t file_size_;
public:
    Memory();
    const uint8_t* getData() const;
    std::size_t getSize() const;
	uint32_t ReadBigEndianInt32(const size_t& addr) const;
	void clear();
};
