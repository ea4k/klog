# Minimal FindHamlib.cmake
# Provides imported target Hamlib::Hamlib when found.

include_guard()

set(_hamlib_hints
  $ENV{HAMLIB_ROOT}
  ${HAMLIB_ROOT}
  ${Hamlib_ROOT}
  "C:/deps/hamlib"
  "/usr"
  "/usr/local"
  "/opt/homebrew"
)

find_path(Hamlib_INCLUDE_DIR
  NAMES hamlib/rig.h
  HINTS ${_hamlib_hints}
  PATH_SUFFIXES include
)

find_library(Hamlib_LIBRARY
  NAMES hamlib libhamlib
  HINTS ${_hamlib_hints}
  PATH_SUFFIXES lib lib64
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Hamlib
  REQUIRED_VARS Hamlib_INCLUDE_DIR Hamlib_LIBRARY
)

if(Hamlib_FOUND AND NOT TARGET Hamlib::Hamlib)
  add_library(Hamlib::Hamlib UNKNOWN IMPORTED)
  set_target_properties(Hamlib::Hamlib PROPERTIES
    IMPORTED_LOCATION "${Hamlib_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${Hamlib_INCLUDE_DIR}"
  )
endif()