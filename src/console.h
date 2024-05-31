#pragma once

#include "processes/os.h"


class Console {
public:
    Console(): os() {};
    void start(std::string file) {
        os.startup(file);
    }
    OS os;
};