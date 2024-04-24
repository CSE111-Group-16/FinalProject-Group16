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

    // Public member functions
    void setAddress(long value); // Function to set privateInt
    void setName(const std::string& value); // Function to set privateString
    long getAddress() const; // Function to get privateInt
    const std::string& getName() const; // Function to get privateString
};

// Default constructor definition
Register::Register() {
    address = 0;
    registername = "x0";
}

// Parameterized constructor definition
Register::Register(long addrValue, const std::string& strValue) {
    address = addrValue;
    registername = strValue;
}

// Destructor definition
Register::~Register() {
    // Destructor code here (if needed)
}

// Function to set address
void Register::setAddress(long value) {
    address = value;
}

