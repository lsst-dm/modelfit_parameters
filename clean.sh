# Delete the build directory
rm -rf build-release
# Clean the docs
if command -v package-docs &> /dev/null
then
  package-docs -d doc clean
fi
if command -v python &> /dev/null
then
  if command -v scons &> /dev/null
    then
      if !(scons --clean); then
        echo "scons --clean failed; continuing on"
      fi
  fi
fi
