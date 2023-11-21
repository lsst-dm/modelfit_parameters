# Configure package to install to a local conda folder
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$CONDA_PREFIX/.local/lib64/pkgconfig
./setup.sh "--prefix=$CONDA_PREFIX/.local"
