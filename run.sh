rm -rf build

mkdir build && cd build

cmake ..

make

./describe_object circle 12
./describe_object triangle 12 5
