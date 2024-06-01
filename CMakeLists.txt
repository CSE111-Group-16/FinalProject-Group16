
cmake_minimum_required(VERSION 3.22.1)

project(BananaEmulator)

set(CMAKE_CXX_STANDARD 17)

# executables
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/processes/console.cpp
    src/hardware/CPU/CPU.cpp
    src/hardware/GPU/gpu.cpp
    src/hardware/CPU/register.cpp
    src/hardware/memory.cpp
)

# SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
