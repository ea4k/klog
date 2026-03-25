set(HAMLIB_ROOT "" CACHE PATH "Root directory of a Hamlib installation (Windows), e.g. C:/hamlib-w64-<ver>")

if(WIN32)
  set(_hamlib_hints)

  if(DEFINED ENV{HAMLIB_ROOT})
    list(APPEND _hamlib_hints "$ENV{HAMLIB_ROOT}")
  endif()

  if(HAMLIB_ROOT)
    list(APPEND _hamlib_hints "${HAMLIB_ROOT}")
  endif()

  # Auto-discover typical installation under C:/hamlib-w64-<version>
  file(GLOB _hamlib_roots "C:/hamlib-w64-*")
  list(APPEND _hamlib_hints ${_hamlib_roots})

  list(REMOVE_DUPLICATES _hamlib_hints)
endif()

find_path(Hamlib_INCLUDE_DIR
  NAMES hamlib/rig.h
  HINTS ${_hamlib_hints}
  PATH_SUFFIXES include .
  NO_DEFAULT_PATH
  DOC "Path to Hamlib headers"
)

find_library(Hamlib_LIBRARY
  NAMES hamlib libhamlib hamlib-4 hamlib-5 hamlib-6
  HINTS ${_hamlib_hints}
  PATH_SUFFIXES lib lib64 .
  NO_DEFAULT_PATH
  DOC "Path to Hamlib library"
)

include(LibFindMacros)

libfind_pkg_detect(Hamlib hamlib
  FIND_PATH hamlib/rig.h PATH_SUFFIXES hamlib
  FIND_LIBRARY hamlib
)

libfind_process(Hamlib)

if(NOT Hamlib_PKGCONF_FOUND)
  if(WIN32)
    set(Hamlib_LIBRARIES ${Hamlib_LIBRARIES};ws2_32)
  else()
    set(Hamlib_LIBRARIES ${Hamlib_LIBRARIES};m;dl)
  endif()
elseif(UNIX AND NOT APPLE)
  set(Hamlib_LIBRARIES ${Hamlib_PKGCONF_STATIC_LDFLAGS})
endif()

# Fix up extra link libraries for macOS frameworks
unset(_next_is_framework)
unset(_Hamlib_EXTRA_LIBS)
foreach(_lib IN LISTS Hamlib_LIBRARIES Hamlib_PKGCONF_LDFLAGS)
  if(_next_is_framework)
    list(APPEND _Hamlib_EXTRA_LIBS "-framework ${_lib}")
    unset(_next_is_framework)
  elseif(${_lib} STREQUAL "-framework")
    set(_next_is_framework TRUE)
  else()
    list(APPEND _Hamlib_EXTRA_LIBS ${_lib})
  endif()
endforeach()

if(Hamlib_FOUND AND NOT TARGET Hamlib::Hamlib)
  add_library(Hamlib::Hamlib UNKNOWN IMPORTED)
  set_target_properties(Hamlib::Hamlib PROPERTIES
    IMPORTED_LOCATION "${Hamlib_LIBRARY}"
    INTERFACE_COMPILE_OPTIONS "${Hamlib_PKGCONF_STATIC_OTHER}"
    INTERFACE_INCLUDE_DIRECTORIES "${Hamlib_INCLUDE_DIR}"
    INTERFACE_LINK_LIBRARIES "${_Hamlib_EXTRA_LIBS}"
  )
endif()

mark_as_advanced(
  Hamlib_INCLUDE_DIR
  Hamlib_LIBRARY
  Hamlib_LIBRARIES
)
