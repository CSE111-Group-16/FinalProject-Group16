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
    
    // copy data to RAM (90% sure working)
    for (size_t i=0; i<data_size; i++) {
        uint8_t rom_byte = memory.readByte(load_data_address + i);
        memory.setByte(program_data_address + i, rom_byte);
    }

    // set stack pointer reg to end of stack
    cpu.resetStackPointer();

    // call setup()
    setup();

    cpu.resetStackPointer();
    loop();
    file.close();
    // start game loop() TODO (but prob later)
}

void OS::loop() {
    std::cout<< "loop()====================\n";
    // Set up initial state
    cpu.PC = 0xfffc; // Set PC register to 0xfffc
    cpu.initialJAL(address_to_loop); // Set CPU to run this instruction
    std::cout << "initial " << std::hex << cpu.PC << std::endl;

    if (cpu.PC == 0x0000 || cpu.PC < 0x8000) {
            cpu.PC = 0xfffc;
            cpu.initialJAL(address_to_loop);
    }

    // Loop until the PC reaches 0x0000 or goes below 0x8000
    while (true) { 
        uint32_t instruction = readInt32(cpu.PC);
        //std::cout << std::hex << instruction << std::endl;
        std::cout << std::hex << cpu.PC << std::endl;
        cpu.PerformInstruction(instruction); // runs next instruction until PC == 0x0000
        std::cout << std::hex << cpu.PC << std::endl;
        // if reset loop
        if (cpu.PC <= 0x0000) {
            std::cout<<"should reset loop\n";

            // not sure if supposed to break or reset loop here
            //70% sure its break tho
            break;
            //cpu.PC = 0xfffc;
            //cpu.initialJAL(address_to_loop);
        }
        if (exitCondition) break;
        std::cout<<"endloop iter\n";
    }
    std::cout<< "\nend loop()================\n";

}

void OS::setup() {
    // setup() psuedo code for now:
    std::cout<<"startup()===============\n";
    cpu.PC = 0xfffc; // set PC register to 0xfffc
    cpu.initialJAL(address_to_setup); // set CPU to run this instruction
    
    while (cpu.PC != 0x0000) { // when PC reg returns to 0x0000, setup() is finished
        uint32_t instruction = readInt32(cpu.PC);
        //std::cout<<std::hex<<instruction<<std::endl;

        cpu.PerformInstruction(instruction); // runs next instruction until PC == 0x0000
        // stopp
        if (cpu.PC < 0x8000) break;
        if (exitCondition) break;
    }
    exitCondition = false;
    std::cout<<"\nend startup()===============\n";

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