#!/bin/bash
set -e # To quit on first error
ARGS=$1 # For running specific tests

if [[ "$1" == "clean" ]]; then
	echo "Cleaning software..."
	make clean
fi
make
make lib

cd tests/
make clean
make

mkdir -p temp
./tests.o $ARGS

echo "All tests succeded!"
