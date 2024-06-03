project(
  'lsst_modelfit_parameters',
  'cpp',
  version : files('VERSION'),
  license : 'GPL-3.0-or-later',
  default_options : ['cpp_std=c++17', 'warning_level=3']
)
project_description = 'Define parameters with limits and transforms for model fitting'

# Target
public_headers = include_directories('include')

# Project
# Make this library usable as a Meson subproject.
project_dep = declare_dependency(
    include_directories: public_headers
)
set_variable(meson.project_name() + '_dep', project_dep)

# Make this library usable from the system's package manager.
subdir('include')

eups = find_program('eups', required: false)
has_eups = eups.found()
if has_eups
    has_eups = run_command('eups', 'list', 'modelfit_parameters', check: false).returncode() == 0
endif

if not has_eups
    pkg_mod = import('pkgconfig')
    pkg_mod.generate(
        name : meson.project_name(),
        filebase : meson.project_name(),
        description : project_description,
    )
endif

# Unit Tests
subdir('tests')

# Build package docs (optional)
subdir('doc')

# Build doxygen docs (done by eupspkg install if has_eups)
if not has_eups
    pymod = import('python')
    python = pymod.find_installation(
        'python3',
        required : false,
        modules: ['lsst.sconsUtils'],
    )
    has_py3 = python.found()
    scons = find_program('scons', required: false)
    has_scons = scons.found()
    if has_scons and has_py3
        message('Running SCons to build docs; see ' + meson.build_root() + '/meson-logs/meson-log.txt for output')
        run_command('sh', '-c', 'cd $MESON_SOURCE_ROOT && echo $PWD && scons', check: false, capture: true)
    endif
endif
