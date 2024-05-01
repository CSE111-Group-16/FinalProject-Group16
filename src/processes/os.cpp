#include "os.h"

void OS::startup(std::string filename) {
    filename_ = filename;
    resetSequence();
}

void OS::resetSequence() {
    // clearing RAM with zeros;
    memory.clearRAM();
    
    // opening ROM file
    std::ifstream file;
    file.open(filename_, std::ios::ate);

    // check if file opened successfully
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename_ << std::endl;
        return;
    }

    // saving file size
    rom_size_ = file.tellg();

    // set file pointer to beginning
    file.seekg(0, std::ios::beg);

    std::unique_ptr<char[]> read_rom;
    read_rom = std::make_unique<char[]>(rom_size_);

    // read rom into rom_contents_ and memory (if possible)
    file.read(read_rom.get(), rom_size_);

    // load ROM into memory
    memory.loadROM(read_rom.get(), rom_size_); // put ROM into memory (at address 0x8000);

    // get relevant info from ROM
    address_to_setup = readInt32(0x81e0);
    address_to_loop = readInt32(0x81e4);
    load_data_address = readInt32(0x81e8);
    program_data_address = readInt32(0x81ec);
    data_size = readInt32(0x81f0);
    
    // copy data to RAM (TEST)
    for (size_t i=0; i<data_size; i++) {
        uint8_t rom_byte = memory.readByte(load_data_address + i);
        memory.setByte(program_data_address + i, rom_byte);
    }

    // set stack pointer reg to end of stack
    // cpu.SetUpRegisters(); // pretty sure stack pointer is set to 0x3400 here

    // call setup()
    setup();
    file.close();
    // start game loop() TODO (but prob later)
    // loop();
}

void OS::setup() {
    std::cout << "setup() starts at: " << address_to_setup << std::endl;
    std::cout << "setup() address read to be: " << readInt32(0x81e0) << std::endl;
    
    // setup() psuedo code for now:
    /*
    cpu.PC = 0xfffc; // set PC register to 0xfffc
    cpu.JAL(address_to_setup); // set CPU to run this instruction
    size_t i = 1; // start at 1 since above line was first instruction
    while (cpu.PC != 0x0000) { // when PC reg returns to 0x0000, setup() is finished
        cpu.runInstruction(address_to_setup+(4*i)); // runs next instruction until PC == 0x0000
    }
    */
    
    // TODO
}


uint32_t OS::readInt32(const size_t& address) const {
    uint32_t out = 0;
    for (int i = 0; i < 4; i++) {
        out <<= 8;
        out |= (uint8_t)memory.readByte(address+i);
    }
    return out;
}

uint16_t OS::readInt16(const size_t& address) const {
    uint32_t out = 0;
    for (int i = 0; i < 2; i++) {
        out <<= 8;
        out |= (uint8_t)memory.readByte(address+i);
    }
    return out;
}

uint8_t OS::readInt8(const size_t& address) const {
    return (uint8_t)memory.readByte(address);
}