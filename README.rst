Modelfit Parameters
###################

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
with the Rubin/LSST `Science Pipelines <https://pipelines.lsst.io/>`_,
call:

.. code-block:: sh
   :name: build
   setup -jr .
   eupspkg -e -v 1 config
   eupspkg -e -v 1 build

If testing a ticket with Jenkins, see full instructions with more steps in the
`developer guide <https://developer.lsst.io/stack/packaging-third-party-eups-dependencies.html#testing-the-package>`_.

Doc build
#########

Sphinx package documentation is built by meson using
`documenteer <https://github.com/lsst-sqre/documenteer/>`_, if available.
Since this module has no Python bindings of its own (yet), these docs are not
very useful.

C++ Doxygen docs are built with scons using
`sconsUtils <https://github.com/lsst/sconsUtils>`_, if available, either by
calling eupspkg install or manually running scons.
sconsUtils' ``tickets/DM-44144`` branch can be pip-installed outside of the
Science Pipelines (the next section).

Standalone builds
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
