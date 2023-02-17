project(
  'parameters',
  'cpp',
  version : '0.1.0',
  license : 'GPL-3.0-or-later',
  default_options : ['warning_level=3']
)
project_description = 'Define parameters with limits and transforms for parametric modelling'

project_test_files = [
  'tests/parameter.cc',
]

build_args = [
]

# ===================================================================

# ======
# Target
# ======

public_headers = include_directories('include')

project_target = shared_library(
  meson.project_name(),
  [],
  install : false,
  c_args : build_args,
  gnu_symbol_visibility : 'hidden',
  include_directories : public_headers,
)


# =======
# Project
# =======

# Make this library usable as a Meson subproject.
project_dep = declare_dependency(
  include_directories: public_headers
)
set_variable(meson.project_name() + '_dep', project_dep)

# Make this library usable from the system's
# package manager.
subdir('include')

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
#  subdirs : meson.project_name(),
)


# ==========
# Unit Tests
# ==========
subdir('tests')
