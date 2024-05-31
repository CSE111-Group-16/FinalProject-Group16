#ifndef REGISTER_H
#define REGISTER_H
#pragma once

#include <string>

class Register {
private:
	int16_t value_;
    std::string registerName;
public:
    // Constructors
    Register(); // Default constructor
    Register(int16_t addrValue, const std::string& strValue); // Parameterized constructor

    // Public member functions
    void setValue(int16_t value); // Function to set value
    void setName(const std::string& name); // Function to set register name
    int16_t getValue() const; // Function to get value
    const std::string getName() const; // Function to get register name
};

#endif // REGISTER_H
