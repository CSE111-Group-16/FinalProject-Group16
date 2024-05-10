#!bin/bash

make
../build/BananaEmulator ../../tests/slug_files/hello_world1.slug > results.txt

../build/BananaEmulator ../../tests/slug_files/hello_world2.slug >> results.txt

../build/BananaEmulator ../../tests/slug_files/hello_world3.slug < ../../local-rom/tests/hello_world3/0.in >> results.txt

cat results.txt