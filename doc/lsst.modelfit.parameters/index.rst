.. py:currentmodule:: lsst.modelfit.parameters

.. _lsst.modelfit.parameters:

########################
lsst.modelfit.parameters
########################

modelfit_parameters provides interfaces for parameters with floating point
values, limits, transforming functions (transforms), and some associated
metadata like names, descriptions, units with string names, labels for
individual parameters, etc. These are intended for use in parametric model
fitting, where parameters may be fixed or free (which indicates that they are
to be optimized).

.. _lsst.modelfit.parameters-using:

Using lsst.modelfit.parameters
==============================

Example usage can be found in the unit tests.

.. toctree::
   :maxdepth: 2

.. _lsst.modelfit.parameters-contributing:

Contributing
============

``lsst.modelfit.parameters`` is developed at https://github.com/lsst/modelfit_parameters.
You can find Jira issues for this module under the `modelfit_parameters <https://rubinobs.atlassian.net/issues/?jql=project%20%3D%20DM%20AND%20component%20%3D%20modelfit_parameters>`_ component.

.. If there are topics related to developing this module (rather than using it), link to this from a toctree placed here.

.. .. toctree::
..    :maxdepth: 2

.. .. _lsst.modelfit.parameters-pyapi:

Python API reference
====================

``lsst.modelfit.parameters`` has no Python bindings (yet), as no standard
Parameter classes are included. Packages defining parameters should include
their own bindings.

.. .. automodapi:: lsst.modelfit.parameters
..   :no-main-docstr:
..   :no-inheritance-diagram:
