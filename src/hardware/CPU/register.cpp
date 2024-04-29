#include <iostream>
#include <string>

class Register {
private:
    // Private member variables
    long address;
    std::string registerName;

public:
    // Constructors
    Register(); // Default constructor
    Register(long address, const std::string& registerName); // Parameterized constructor

    // Destructor
    ~Register();

    // Public member functions
    void setAddress(long address); // Function to set privateInt
    void setName(const std::string& address); // Function to set privateString
    long getAddress() const; // Function to get privateInt
    const std::string& getName() const; // Function to get privateString
};

// Default constructor definition the default reg is x0
Register::Register() {
    address = 0;
    registerName = "x0";
}

// Parameterized constructor definition
Register::Register(long addrValue, const std::string& strValue) {
    address = addrValue;
    registerName = strValue;
}

// Destructor definition
Register::~Register() {
    // Destructor code here (if needed)
}

// Function to set address
void Register::setAddress(long value) {
    address = value;
}

