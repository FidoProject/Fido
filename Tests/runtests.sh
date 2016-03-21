#!/bin/bash
cd ../Software
if [[ "$1" == "clean" ]]; then
	echo "Cleaning software..."
	make clean
fi
make
make lib

cd ../Tests
if [[ "$1" == "clean" ]]; then
	echo "Cleaning tests..."
	make clean
fi
make
./test
