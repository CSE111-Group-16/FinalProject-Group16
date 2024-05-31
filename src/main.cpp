#include <iostream>
#include <filesystem>
#include <SDL2/SDL.h>
#include "processes/os.h"
#include "hardware/CPU/CPU.h"
#include "hardware/memory.h"

int main(int argc, char* argv[]) {
	if (argc != 2) { std::cout << "Provide only a path to a valid slug file" << std::endl; }
	std::string path = argv[1];
	// https://en.cppreference.com/w/cpp/filesystem/exists
	// check if the file exists:
	std::filesystem::path filepath = path;
	bool filepathExists = std::filesystem::exists(filepath);
	if (!filepathExists) { std::cout << "Provide only a path to a valid slug file" << std::endl; }
	if (filepath.extension() != ".slug") { std::cout << "Provide only a path to a valid slug file" << std::endl; }
	OS os;
	os.startup(path);
	SDL_Init(SDL_INIT_VIDEO);
	return 0;
}