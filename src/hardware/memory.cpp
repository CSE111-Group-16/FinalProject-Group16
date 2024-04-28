#include "src/hardware/memory.h"

#include <fstream>
#include <iostream>

Memory::Memory(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error opening file\n";
        return;
    }

    file_size_ = file.tellg();
    file.seekg(0, std::ios::beg);

    contents_ = std::make_unique<char[]>(file_size_);
    if (!file.read(contents_.get(), file_size_)) {
        std::cerr << "Error reading file\n";
        return;
    }

    file.close();
}

const char* Memory::getData() const {
    return contents_.get();
}

std::size_t Memory::getSize() const {
    return file_size_;
}