#ifndef REGISTER_H
#define REGISTER_H
#pragma once

#include <string>

class Register {
public:
    int16_t address; // should really be named 'value' or something
    std::string registerName;

    // Constructors
    Register(); // Default constructor
    Register(int16_t addrValue, const std::string& strValue); // Parameterized constructor

    // Public member functions
    void setAddress(int16_t value); // Function to set address
    void setName(const std::string& value); // Function to set register name
    int16_t getAddress() const; // Function to get address // pretty sure it should just be value
    const std::string getName() const; // Function to get register name
};

#endif // REGISTER_H
