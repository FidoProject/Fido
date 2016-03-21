cd ../Software
rm foo.o
rm fido-lib.a
make
make lib

cd ../Tests
rm test
make
./test
