#include <iostream>
#include <string>
#include "register.h"

// Default constructor definition the default reg is x0
Register::Register() {
    value_ = 0;
    registerName = "x0";
}

// Parameterized constructor definition
Register::Register(int16_t addrValue, const std::string& strValue) {
    value_ = addrValue;
    registerName = strValue;
}

void Register::setValue(int16_t value) {
    value_ = value;
}

void Register::setName(const std::string& name) {
	registerName = name;
}

int16_t Register::getValue() const {
    if (registerName == "zero") return 0;
    if (registerName == "sp") {
        if (value_ > 0x3400) {
            std::cerr << "Stack Overflow. Stack pointer at: " << value_ << std::endl;
            exit(EXIT_FAILURE);
        }
        if (value_ < 0x1400) {
            std::cerr << "Stack Overflow. Stack pointer at: " << value_ << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    return value_;
}

const std::string Register::getName() const {
    return registerName;
}
