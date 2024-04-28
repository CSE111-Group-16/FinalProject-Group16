#include <iostream>
#include <string>
#include "register.h"

class CPU {
private:
    // Private member variables
    register* registerFile;
    register PC;

public:
    // Constructors
    CPU(); // Default constructor
    CPU(register* registerFile, register PC); // Parameterized constructor

    // Destructor
    ~CPU();
}