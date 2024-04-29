# EupsPkg config file. Sourced by 'eupspkg'

build()
{
    ./build.sh
}

clean()
{
    ./clean.sh
}

config()
{
    ([ -d "$MODELFIT_PARAMETERS_DIR" ] && ./clean.sh && meson setup --prefix="$MODELFIT_PARAMETERS_DIR/build-release" \
     --buildtype release build-release)
}

