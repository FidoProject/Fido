cd ../Software
make clean
make
make lib

cd ../Tests
make clean
make
./test
