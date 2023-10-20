# EupsPkg config file. Sourced by 'eupspkg'

build()
{
    ./build.sh
}

config()
{
    ([ -d "$PARAMETERS_DIR" ] && ./clean.sh && meson setup --prefix="$PARAMETERS_DIR/build-release" \
     --buildtype release build-release)
}

