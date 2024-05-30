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

#define WINDOW_WIDTH 128
#define WINDOW_HEIGHT 120

class OS {
public:
    // default constructor
    OS() : memory(this), cpu(this), gpu(this) {
        // initialize sdl
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            printf("error initializing SDL: %s\n", SDL_GetError());
        }
        win = SDL_CreateWindow("GAME",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                WINDOW_WIDTH, WINDOW_HEIGHT, 0);

        renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        texture = SDL_CreateTexture(renderer,
                                         SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_STREAMING,
                                         WINDOW_WIDTH, WINDOW_HEIGHT);                
        };

    // accessable to user
    void startup(std::string ROM_file); 
    void shutDown(); // not sure if needed
    ~OS() {
        logger.close();
    	SDL_DestroyWindow(win);
        SDL_Quit();
        }
    // hardware
    CPU cpu;
    Memory memory;
    GPU gpu;
    std::ofstream logger;
    SDL_Window* win;
    SDL_Surface* screen;
    SDL_Event eventHandler;
    SDL_Renderer* renderer;
    SDL_Texture* texture;


    // ROM contents (might remove if we can get it in memory)
    // std::unique_ptr<char[]> rom_contents_; // maybe remove later if storing ROM in memory
    
    // values
    uint32_t address_to_setup;
    uint32_t address_to_loop;
    uint32_t load_data_address;
    uint32_t program_data_address;
    uint32_t data_size;
    int controllerByte;
    bool exitCondition = false;
    bool pressedA, pressedB, pressedSelect, pressedStart;


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
    void eventLoop();


private:
    bool logInstruction = false;
    bool logPCLocation = false;
};