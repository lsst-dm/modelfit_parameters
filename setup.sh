# Configure and build
meson setup "$@" --buildtype release build-release
./build-cc.sh
