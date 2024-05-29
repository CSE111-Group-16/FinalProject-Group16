#pragma once

#include "../hardware/CPU/CPU.h"
#include "../hardware/GPU/gpu.h"
#include "../hardware/memory.h"
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <SDL2/SDL.h>

// pulled directly from doc
#define CONTROLLER_A_MASK ((uint8_t)0x80)
#define CONTROLLER_B_MASK ((uint8_t)0x40)
#define CONTROLLER_SELECT_MASK ((uint8_t)0x20)
#define CONTROLLER_START_MASK ((uint8_t)0x10)
#define CONTROLLER_UP_MASK ((uint8_t)0x08)
#define CONTROLLER_DOWN_MASK ((uint8_t)0x04)
#define CONTROLLER_LEFT_MASK ((uint8_t)0x02)
#define CONTROLLER_RIGHT_MASK ((uint8_t)0x01)

class OS {
public:
    // default constructor
    OS() : memory(this), cpu(this), gpu(this) {};

    // accessable to user
    void startup(std::string ROM_file); 
    void shutDown(); // not sure if needed
    ~OS() {
        logger.close();
        SDL_Quit();
        }
    // hardware
    CPU cpu;
    Memory memory;
    GPU gpu;
    std::ofstream logger;


    // ROM contents (might remove if we can get it in memory)
    // std::unique_ptr<char[]> rom_contents_; // maybe remove later if storing ROM in memory
    
    // values
    uint32_t address_to_setup;
    uint32_t address_to_loop;
    uint32_t load_data_address;
    uint32_t program_data_address;
    uint32_t data_size;
    bool exitCondition = false;


    // memory accessors (? idk if needed in os)
    uint32_t readInt32(const size_t& address) const;
    uint16_t readInt16(const size_t& address) const;
    uint8_t readInt8(const size_t& address) const;

    private:

    // file info
    std::string filename_;
    size_t rom_size_;

    // official internal functions
    void resetSequence();
    void setup();
    void loop();

private:
    bool logInstruction = true;
    bool logPCLocation = true;
};