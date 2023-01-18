Parameters
#######

*parameters* is a library for defining parameters used in fitting
models to data. It is being developed primarily for use in astronomy - 
specifically, by
`Vera C. Rubin Observatory Data Management <https://www.lsst.org/about/dm>`_ 
for the `Legacy Survey of Space and Time <https://www.lsst.org/about>`_, and the
`MultiProFit <https://github.com/lsst-dm/multiprofit/>`_ source modelling 
package - but is not domain specific.

*parameters* is build with `Meson <https://github.com/mesonbuild>`. To 
create a build directory, call:

meson builddir/

*parameters* also uses pkg-config to store package information. You will likely
want to configure it to install in a local directory. For example, if you are
using a conda environment:

PKG_CONFIG_PATH=$CONDA_PREFIX/.local/lib64/pkgconfig meson --prefix=$CONDA_PREFIX/.local builddir/

A full example setup script is provided in ``setup_conda.sh``.
This defaults to  using ``$CONDA_PREFIX``, but can be configured to output
elsewhere (e.g. ``~/.local``):

``CONDA_PREFIX=~ sh setup-conda.sh``

Once the build command is run once to create the build directories, subsequent
rebuilds can use the provided ``build.sh`` script.

Note: the default meson build directory is builddir/ (build-release is used in
setup-conda.sh), to disambiguate it with the command `meson build`. However,
many IDEs expect a build directory in build/, as is typical with `cmake`.
It may be convient to create a symbolic link between them, e.g.
`ln -s build-release build`.

