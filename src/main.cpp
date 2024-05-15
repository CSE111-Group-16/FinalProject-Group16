#include <iostream>
#include <filesystem>
#include <SDL2/SDL.h>
#include "tester.h"
#include "processes/os.h"
#include "hardware/CPU/CPU.h"
#include "hardware/memory.h"

int main(int argc, char* argv[]) {
	if (2 == argc) {
		SDL_Init(SDL_INIT_VIDEO);
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
	SDL_Quit();
    return 0;
}