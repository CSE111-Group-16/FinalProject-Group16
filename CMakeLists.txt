
cmake_minimum_required(VERSION 3.22.1)

project(BananaEmulator)

set(CMAKE_CXX_STANDARD 17)

# executables
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/hardware/CPU/CPU.cpp
    src/hardware/CPU/register.cpp
    src/hardware/memory.cpp
    src/processes/os.cpp
    src/tester.cpp
)