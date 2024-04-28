#pragma once

#include <vector>
#include <string>

class Memory {
private:
    std::unique_ptr<char[]> contents_;
    std::size_t file_size_;
public:
    Memory(const std::string& filename);
    const char* getData() const;
    std::size_t getSize() const;
};
