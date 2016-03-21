#!/bin/bash
# This script will install the fido compiler and library on a Unix based system.
echo "Making library..."
cd Software/

make
make lib

if [[ "$1" == "-noSFML" ]]; then
	mv ../temp/FidoKin FidoKin
	mv ../temp/FidoSim FidoSim
	mv ../temp/Tasks/* Tasks
	rm -r ../temp
	# Re-add SFML libraries to Makefile
	mv Makefile-backup Makefile
fi

echo "Made library"
echo "Making fido compiler"

echo "" >> ~/.bash_profile
echo "# Fido compiler, do not change" >> ~/.bash_profile
echo "alias fcc='g++ -std=c++11 $(pwd)/fido-lib.a'" >> ~/.bash_profile
echo "Made compiler"
echo "Compiler is called fcc"
echo "Usage: fcc input.cpp -o a.out"
echo "This would produce executable a.out from source file input.cpp."

source ~/.bash_profile

cd ../
