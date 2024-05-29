#include <iostream>
#include <filesystem>
#include "tester.h"
#include "processes/os.h"
#include "hardware/CPU/CPU.h"
#include "hardware/memory.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 128
#define WINDOW_HEIGHT 120

int main(int argc, char* argv[]) {
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("GAME",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	if (2 == argc) {
		std::string path = argv[1];
		// https://en.cppreference.com/w/cpp/filesystem/exists
		// check if the file exists:
		std::filesystem::path filepath = path;
		bool filepathExists = std::filesystem::exists(filepath);
		if (filepathExists) {
			// check if its a slug file:
			if (filepath.extension() == ".slug") {
				OS os;
				os.startup(path);
				
			}
			else {
				std::cout << "Provide only a path to a valid slug file" << std::endl;
			}
		}
		else {
			std::cout << "Provide only a path to a valid slug file" << std::endl;
		}
	}
	else {
		std::cout << "Provide only a path to a valid slug file" << std::endl;
	}

	SDL_DestroyWindow(win);
	
	return 0;
}