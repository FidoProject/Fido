#!/bin/bash
set -e

cd ../Software
if [[ "$1" == "clean" ]]; then
	echo "Cleaning software..."
	make clean
fi
make
make lib

cd ../Tests
make clean
make
./tests.o
