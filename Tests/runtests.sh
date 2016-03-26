#!/bin/bash
<<<<<<< HEAD
set -e # To quit on first error
ARGS=$1 # For running specific tests
=======
set -e
>>>>>>> FidoProject/master

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
	ARGS=""
fi
make
./test $ARGS

echo "All tests succeded!"
