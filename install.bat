@ECHO OFF
ECHO "Making library..."
DIR Software\
:: Check for arg
IF [%1]==["-noSFML"] GOTO NOSFML
GOTO MAKE

:: First noSFML process
:NOSFML

GOTO MAKE

:: Make the libraries
:MAKE
make
make lib
IF [%1]==["-noSFML"] GOTO NOSFMLTWO
GOTO DONE

:: Second noSFML process
:NOSFMLTWO

GOTO DONE

:: Everything else
:DONE
ECHO "Made library"
ECHO "In order, to compile, simply run a command with g++ which links the library"
ECHO "For example:"
ECHO "g++ -std=c++11 in.cpp PATH_TO_FIDO\Software\fido-lib.a"
