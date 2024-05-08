#include <iostream>
#include <string>
#include "register.h"

// why not in header file?
// class Register {
// private:
//     // Private member variables
//     size_t address;
//     std::string registerName;

// public:
//     // Constructors
//     Register(); // Default constructor
//     Register(uint16_t address, const std::string& registerName); // Parameterized constructor

//     // Destructor
//     ~Register();

//     // Public member functions
//     void setAddress(uint16_t address); // Function to set privateInt
//     void setName(const std::string& address); // Function to set privateString
//     uint16_t getAddress() const; // Function to get privateInt
//     const std::string& getName() const; // Function to get privateString
// };

// Default constructor definition the default reg is x0
Register::Register() {
    address = 0;
    registerName = "x0";
}

// Parameterized constructor definition
Register::Register(uint16_t addrValue, const std::string& strValue) {
    address = addrValue;
    registerName = strValue;
}

// Destructor definition
Register::~Register() {
    // Destructor code here (if needed)
}

// Function to set address
void Register::setAddress(uint16_t value) {
    address = value;
}

// getAddress() might not be returning usable address? returns value not reference
uint16_t Register::getAddress() const {
    if (registerName == "zero") return 0;
    if (registerName == "sp") {
        if (address > 0x3400) {
            std::cerr << "Stack Overflow. Stack pointer at: " << address << std::endl;
            exit(EXIT_FAILURE);
        }
        if (address < 0x1400) {
            std::cerr << "Stack Overflow. Stack pointer at: " << address << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    return address;
}

const std::string Register::getName() const {
    return registerName;
}


