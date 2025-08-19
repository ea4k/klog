# Minimal FindHamlib.cmake
# Provides imported target Hamlib::Hamlib when found.
message(STATUS "Loaded FindHamlib.cmake")
include_guard()

set(_hamlib_hints
  $ENV{HAMLIB_ROOT}
  ${HAMLIB_ROOT}
  ${Hamlib_ROOT}
  "C:/Users/devel/Documents/GitHub/libs/win64/hamlib"
  "/usr"
  "/usr/local"
  "/opt/homebrew"
  "/usr/lib/x86_64-linux-gnu"

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

# Back-compat variables expected by some CMakeLists
if(Hamlib_FOUND)
  set(HAMLIB_INCLUDE_DIRS "${Hamlib_INCLUDE_DIR}")
  set(HAMLIB_LIBRARIES    "${Hamlib_LIBRARY}")
endif()
