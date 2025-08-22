# --- Debug: confirm this Find module is being used ---
message(STATUS "[KLog] Using FindHamlib.cmake from: ${CMAKE_CURRENT_LIST_FILE}")

set(HAMLIB_ROOT "" CACHE PATH "Root directory of a Hamlib installation (Windows), e.g. C:/Program Files/hamlib-w64-<ver>")

# Default hints for all platforms


#if (WIN32)
#  message(STATUS "Win32 detected in FindHamlib.cmake")
#  set(Hamlib_INCLUDE_DIR "C:/Program Files/hamlib-w64-4.6.3/include")
#  find_library(Hamlib_LIBRARY
#    NAMES hamlib libhamlib hamlib-4 usb gcc_s winpthread
#    PATHS "C:/Program Files/hamlib-w64-4.6.3/lib/gcc/"
#    NO_DEFAULT_PATH
#  )
#  if(NOT Hamlib_LIBRARY)
#    message(FATAL_ERROR "Hamlib library not found in C:/Program Files/hamlib-w64-4.6.3/bin")
#  else()
#    message(STATUS "Hamlib library found: ${Hamlib_LIBRARY}")
#  endif()
#endif ()



# On Windows, auto-discover typical installers under Program Files without requiring a version
if(WIN32)
  set(_hamlib_hints
    $ENV{HAMLIB_ROOT}
    ${HAMLIB_ROOT}
  )

  set(_hamlib_win_roots)

  if(HAMLIB_ROOT)
    list(APPEND _hamlib_win_roots "${HAMLIB_ROOT}")
  endif()
  if(DEFINED ENV{HAMLIB_ROOT})
    list(APPEND _hamlib_win_roots "$ENV{HAMLIB_ROOT}")
  endif()

  # Typical installers: hamlib-w64-<version>
  file(GLOB _pf      "C:/Program Files/hamlib-w64-*")
  file(GLOB _pf_x86  "C:/Program Files (x86)/hamlib-w64-*")

  list(APPEND _hamlib_win_roots ${_pf} ${_pf_x86})
  list(REMOVE_DUPLICATES _hamlib_win_roots)

  # Prefer these Windows roots by putting them first
  if(_hamlib_win_roots)
    list(PREPEND _hamlib_hints ${_hamlib_win_roots})
  endif()
endif()

# --- Recursive search for headers ---
find_path(Hamlib_INCLUDE_DIR
  NAMES hamlib/rig.h
  HINTS ${_hamlib_hints}
  PATH_SUFFIXES include include/hamlib . # Try root and subfolders
  NO_DEFAULT_PATH
  DOC "Path to Hamlib headers"
)

# --- Recursive search for libraries ---
find_library(Hamlib_LIBRARY
  NAMES hamlib libhamlib hamlib-4 hamlib-5 hamlib-6
  HINTS ${_hamlib_hints}
  PATH_SUFFIXES lib lib64 lib/gcc lib/mingw lib/msvc .
  NO_DEFAULT_PATH
  DOC "Path to Hamlib library"
)

include (LibFindMacros)

# Allow users to hint the install root on Windows (only used on WIN32)
set(HAMLIB_ROOT "" CACHE PATH "Root directory of a Hamlib installation (Windows), e.g. C:/Program Files/hamlib-<ver>")

libfind_pkg_detect (Hamlib hamlib
  FIND_PATH hamlib/rig.h PATH_SUFFIXES hamlib
  FIND_LIBRARY hamlib
  )

libfind_process (Hamlib)

if (NOT Hamlib_PKGCONF_FOUND)
  if (WIN32)
    set (Hamlib_LIBRARIES ${Hamlib_LIBRARIES};ws2_32)
  else ()
    set (Hamlib_LIBRARIES ${Hamlib_LIBRARIES};m;dl)
  endif ()
elseif (UNIX AND NOT APPLE)
  set (Hamlib_LIBRARIES ${Hamlib_PKGCONF_STATIC_LDFLAGS})
endif ()

# fix up extra link libraries for macOS as target_link_libraries gets
# it wrong for frameworks
unset (_next_is_framework)
unset (_Hamlib_EXTRA_LIBS)
foreach (_lib IN LISTS Hamlib_LIBRARIES Hamlib_PKGCONF_LDFLAGS)
  if (_next_is_framework)
    list (APPEND _Hamlib_EXTRA_LIBS "-framework ${_lib}")
    unset (_next_is_framework)
  elseif (${_lib} STREQUAL "-framework")
    set (_next_is_framework TRUE)
  else ()
    list (APPEND _Hamlib_EXTRA_LIBS ${_lib})
  endif ()
endforeach ()

if (Hamlib_FOUND AND NOT TARGET Hamlib::Hamlib)
  add_library (Hamlib::Hamlib UNKNOWN IMPORTED)
  set_target_properties (Hamlib::Hamlib PROPERTIES
    IMPORTED_LOCATION "${Hamlib_LIBRARY}"
    INTERFACE_COMPILE_OPTIONS "${Hamlib_PKGCONF_STATIC_OTHER}"
    INTERFACE_INCLUDE_DIRECTORIES "${Hamlib_INCLUDE_DIR}"
    INTERFACE_LINK_LIBRARIES "${_Hamlib_EXTRA_LIBS}"
    )
endif ()

mark_as_advanced (
  Hamlib_INCLUDE_DIR
  Hamlib_LIBRARY
  Hamlib_LIBRARIES
  )
