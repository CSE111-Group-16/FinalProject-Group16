#include <iostream>
#include <string>

class Register {
private:
    // Private member variables
    long address;
    std::string registername;

public:
    // Constructors
    Register(); // Default constructor
    Register(long addrValue, const std::string& strValue); // Parameterized constructor

    // Destructor
    ~Register();
}