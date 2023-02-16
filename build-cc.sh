CXXFLAGS="$CXXFLAGS -O3" meson compile -C build-release && meson test -C build-release && meson install -C build-release
