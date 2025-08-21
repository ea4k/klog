# --- Debug: confirm this Find module is being used ---
message(STATUS "[KLog] Using FindHamlib.cmake from: ${CMAKE_CURRENT_LIST_FILE}")

include(LibFindMacros)

# Allow users to hint the install root on Windows (only used on WIN32)
set(HAMLIB_ROOT "" CACHE PATH "Root directory of a Hamlib installation (Windows), e.g. C:/Program Files/hamlib-<ver>")

# Build Windows-only search hints
set(_hamlib_include_hints)
set(_hamlib_lib_hints)
set(_hamlib_bin_hints)

if(WIN32)
  set(_hamlib_roots)

  if(HAMLIB_ROOT)
    list(APPEND _hamlib_roots "${HAMLIB_ROOT}")
  endif()

  if(DEFINED ENV{HAMLIB_ROOT})
    list(APPEND _hamlib_roots "$ENV{HAMLIB_ROOT}")
  endif()

  # Auto-discover typical installers: hamlib-<version>
  file(GLOB _pf "C:/Program Files/hamlib-*")
  file(GLOB _pf86 "C:/Program Files (x86)/hamlib-*")
  list(APPEND _hamlib_roots ${_pf} ${_pf86})
  list(REMOVE_DUPLICATES _hamlib_roots)

  foreach(_root IN LISTS _hamlib_roots)
    list(APPEND _hamlib_include_hints
      "${_root}/include"
      "${_root}"           # fallback, in case headers live directly under root/hamlib
    )
    list(APPEND _hamlib_lib_hints
      "${_root}/lib"
      "${_root}/lib64"
    )
    list(APPEND _hamlib_bin_hints
      "${_root}/bin"
    )
  endforeach()
endif()

# Try pkg-config when available, but also pass Windows hints for manual search.
libfind_pkg_detect(Hamlib hamlib
  FIND_PATH hamlib/rig.h PATH_SUFFIXES hamlib HINTS ${_hamlib_include_hints}
  FIND_LIBRARY hamlib HINTS ${_hamlib_lib_hints}
)

libfind_process(Hamlib)

# Add platform-specific system libs when pkg-config didn't provide a complete list
if (NOT Hamlib_PKGCONF_FOUND)
  if (WIN32)
    set(Hamlib_LIBRARIES ${Hamlib_LIBRARIES};ws2_32)
  else ()
    set(Hamlib_LIBRARIES ${Hamlib_LIBRARIES};m;dl)
  endif ()
elseif (UNIX AND NOT APPLE)
  set (Hamlib_LIBRARIES ${Hamlib_PKGCONF_STATIC_LDFLAGS})
endif ()

# Fix up extra link libraries for macOS as target_link_libraries gets frameworks wrong
unset(_next_is_framework)
unset(_Hamlib_EXTRA_LIBS)
foreach(_lib IN LISTS Hamlib_LIBRARIES Hamlib_PKGCONF_LDFLAGS)
  if (_next_is_framework)
    list(APPEND _Hamlib_EXTRA_LIBS "-framework ${_lib}")
    unset(_next_is_framework)
  elseif ("${_lib}" STREQUAL "-framework")
    set(_next_is_framework TRUE)
  else()
    list(APPEND _Hamlib_EXTRA_LIBS ${_lib})
  endif()
endforeach()

# Create an imported target with proper properties
if (Hamlib_FOUND AND NOT TARGET Hamlib::Hamlib)
  add_library(Hamlib::Hamlib UNKNOWN IMPORTED)

  # Base include dirs and interface link options
  set_target_properties(Hamlib::Hamlib PROPERTIES
    INTERFACE_COMPILE_OPTIONS "${Hamlib_PKGCONF_STATIC_OTHER}"
    INTERFACE_INCLUDE_DIRECTORIES "${Hamlib_INCLUDE_DIR}"
    INTERFACE_LINK_LIBRARIES "${_Hamlib_EXTRA_LIBS}"
  )

  # Library location properties
  if (WIN32)
    if (EXISTS "${Hamlib_LIBRARY}")
      string(TOLOWER "${Hamlib_LIBRARY}" _hl_lib_lower)
      if (_hl_lib_lower MATCHES "\\.lib$")
        # MSVC import library
        set_target_properties(Hamlib::Hamlib PROPERTIES IMPORTED_IMPLIB "${Hamlib_LIBRARY}")
        # Best-effort: locate the runtime DLL so IDEs can run without PATH tweaks
        find_file(Hamlib_DLL
          NAMES hamlib.dll hamlib-4.dll hamlib-5.dll hamlib-6.dll
          HINTS ${_hamlib_bin_hints}
        )
        if (Hamlib_DLL)
          set_target_properties(Hamlib::Hamlib PROPERTIES IMPORTED_LOCATION "${Hamlib_DLL}")
        else()
          # Fallback: not ideal, but keeps the target valid
          set_target_properties(Hamlib::Hamlib PROPERTIES IMPORTED_LOCATION "${Hamlib_LIBRARY}")
        endif()
      else()
        # MinGW: .dll.a or .a – link directly
        set_target_properties(Hamlib::Hamlib PROPERTIES IMPORTED_LOCATION "${Hamlib_LIBRARY}")
      endif()
    endif()
  else()
    set_target_properties(Hamlib::Hamlib PROPERTIES IMPORTED_LOCATION "${Hamlib_LIBRARY}")
  endif()
endif ()

mark_as_advanced(
  Hamlib_INCLUDE_DIR
  Hamlib_LIBRARY
  Hamlib_LIBRARIES
)
