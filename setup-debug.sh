# Configure package and build debug libraries
meson setup "$@" --buildtype debug build-debug
./build-cc-debug.sh
