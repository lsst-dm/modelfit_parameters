sphinx = find_program(['sphinx-build'], required: false)

if sphinx.found()
  command = [
    # -E -a together force rebuilds
    sphinx, '-E', '-a', '-b', 'html',
    '-Dbreathe_projects.modelfit_parameters=' + meson.build_root() + '/doc/xml',
    meson.current_source_dir() + '/source',
    'doc/sphinx/html'
  ]

  sphinx_target = custom_target('sphinx',
                                output : 'html',
                                command : command,
                                # TODO: why does nothing get build if install = false?
                                install : true,
                                # TODO: why doesn't this work without a subdir?
                                # FileNotFoundError: [Errno 2] No such file or directory:
                                # '.../modelfit_parameters/build-release/doc/sphinx/html/index.html'
                                install_dir : meson.current_build_dir() + '/install'
                                )
endif
