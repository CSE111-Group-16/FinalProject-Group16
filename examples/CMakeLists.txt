
cmake_minimum_required(VERSION 3.22.1)

project(hello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
    hello.cpp
)
