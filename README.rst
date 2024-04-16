Modelfit: Parameters
####################

*modelfit_parameters* is a library for defining parameters used in fitting
parametric models. It is being developed primarily for use in astronomy -
specifically, by
`Vera C. Rubin Observatory Data Management <https://www.lsst.org/about/dm>`_ 
for the `Legacy Survey of Space and Time <https://www.lsst.org/about>`_, and
the `gauss2dfit <https://github.com/lsst-dm/gauss2dfit/>`_ Gaussian mixture
modelling package - but is not domain specific.

Building the Package
####################

*modelfit_parameters* is built with `Meson <https://github.com/mesonbuild>`_.
Although it is currently a header-only library, the build also runs tests
and generates docs, so building should not be skipped.

EUPS build
##########

To build with `eups <https://github.com/RobertLuptonTheGood/eups>`_ for use
with the Rubin/LSST `Science Pipelines <https://https://pipelines.lsst.io/>`_,
call:

.. code-block:: sh
   :name: build
   setup -jr .
   eupspkg -e -v 1 config
   eupspkg -e -v 1 build

Other build types
#################

A full example setup script is provided in ``setup_conda.sh``.
This defaults to  using ``$CONDA_PREFIX``, but can be configured to output
elsewhere (e.g. ``~/.local``) like so:

``CONDA_PREFIX=~ sh setup-conda.sh``

Once the build command is run once to create the build directories, subsequent
rebuilds can use the provided ``build.sh`` script.

Otherwise, to manually create a build directory, call:

``meson builddir/``

*modelfit_parameters* can use pkg-config to store package information.
You will likely want to configure it to install in a local directory.
For example, if you are using a conda environment:

``PKG_CONFIG_PATH=$CONDA_PREFIX/.local/lib64/pkgconfig meson
--prefix=$CONDA_PREFIX/.local builddir/``

Note: the default meson build directory is ``builddir/`` (build-release is
used in ``setup-conda.sh``), to disambiguate with the command ``meson build``.
However, many IDEs expect a build directory in ``build/``, as is typical with
``cmake``. It may be convenient to create a symbolic link between them, e.g. by
``ln -s build-release build``. Alternatively, some IDEs may support opening the
built ``compile_commands.json``, which you may also want to symlink to the
root directory.
