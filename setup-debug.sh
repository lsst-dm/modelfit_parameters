# Configure package and build debug libraries
meson setup --prefix=$PWD/debug --buildtype debug build-debug
./build-cc-debug.sh
