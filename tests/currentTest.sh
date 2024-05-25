#!/bin/bash

# run with:
# $ source currentTest.sh

# cmake 
mkdir -p build
cd build/
cmake ../../

# building executable
make 

# running test application
./BananaEmulator ../slug_files/hello_world1.slug > results.txt
./BananaEmulator ../slug_files/hello_world2.slug >> results.txt
./BananaEmulator ../slug_files/hello_world3.slug < ../slug_files/hello_world3_input.txt >> results.txt

# return to origin
cd ../