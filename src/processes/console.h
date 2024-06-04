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
#include <cstdio> 

#define CONTROLLER_A_MASK ((uint8_t)0x80)
#define CONTROLLER_B_MASK ((uint8_t)0x40)
#define CONTROLLER_SELECT_MASK ((uint8_t)0x20)
#define CONTROLLER_START_MASK ((uint8_t)0x10)
#define CONTROLLER_UP_MASK ((uint8_t)0x08)
#define CONTROLLER_DOWN_MASK ((uint8_t)0x04)
#define CONTROLLER_LEFT_MASK ((uint8_t)0x02)
#define CONTROLLER_RIGHT_MASK ((uint8_t)0x01)

#define ADDRESS_TO_SETUP_ 0x81e0
#define ADDRESS_TO_LOOP_ 0x81e4
#define LOAD_DATA_ADDRESS_ 0x81e8
#define PROGRAM_DATA_ADDRESS_ 0x81ec
#define DATA_SIZE_ 0x81f0
#define LOOP_CALL_ 0xfffc
#define ZERO_ 0x0000

#define WINDOW_WIDTH 128
#define WINDOW_HEIGHT 120

class Console {
public:
    // default constructor
    Console() : memory(this), cpu(this), gpu(this) {
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
    ~Console() {
        stopRecording();
        logger.close();
    	SDL_DestroyWindow(win);
        SDL_Quit();
        }

    SDL_Event getEventHandler();
    bool setExitCondition(bool cond);
    int getControllerByte();
    SDL_Renderer* getRenderer();
    SDL_Texture* getTexturer();
    Memory* getMemory();
	std::ofstream* getLogger();

    void startRecording(const std::string& outputFile);
    void stopRecording();

private:
    // file info
    std::string filename_;
    size_t rom_size_;

    // official internal functions
    void resetSequence();
    void setup();
    void loop();
    void eventLoop();
    bool logInstruction = false;
    bool logPCLocation = false;
    bool exitCondition = false;
    int controllerByte = 0;
	
	// hardware
    CPU cpu;
    GPU gpu;
    SDL_Window* win;
    SDL_Surface* screen;
    SDL_Event eventHandler;
    SDL_Renderer* renderer;
	SDL_Texture* texture;
    std::ofstream logger;
    std::ofstream* loggerP = &logger;
	Memory memory;
	Memory* memoryP = &memory;

	// values
    uint32_t address_to_setup;
    uint32_t address_to_loop;
    uint32_t load_data_address;
    uint32_t program_data_address;
    uint32_t data_size;

    // memory accessors
    uint32_t readInt32(const size_t& address) const;
    uint16_t readInt16(const size_t& address) const;
    uint8_t readInt8(const size_t& address) const;

    std::string ffmpegCommand;
    FILE* ffmpegPipe = nullptr;
};