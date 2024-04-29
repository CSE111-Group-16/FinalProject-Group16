#ifndef REGISTER_H
#define REGISTER_H
#pragma once

#include <string>

class Register {
private:
    // Private member variables
    uint16_t address;
    std::string registername;

public:
    // Constructors
    Register(); // Default constructor
    Register(long addrValue, const std::string& strValue); // Parameterized constructor

    // Destructor
    ~Register();

    // Public member functions
    void setAddress(long value); // Function to set address
    void setName(const std::string& value); // Function to set register name
    long getAddress() const; // Function to get address
    const std::string& getName() const; // Function to get register name
};

#endif // REGISTER_H
