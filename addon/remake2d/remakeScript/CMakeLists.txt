cmake_minimum_required(VERSION 3.32)

project(
	remakeScript
	VERSION 0.1.0
	DESCRIPTION "A remake2d add-on for lua scripting"
	LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type" FORCE)
endif()

string(APPEND CMAKE_CXX_FLAGS_RELEASE " -O2 -DNDEBUG")
string(APPEND CMAKE_CXX_FLAGS_DEBUG   " -Wall -Wextra -Werror")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(LOSLIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/remake2d/lua/loslib.c")

if(EXISTS "${LOSLIB_PATH}")
    file(READ "${LOSLIB_PATH}" FILE_CONTENTS)

    set(OLD_MACRO_REGEX "#define[ \t]+lua_tmpnam\\(b,e\\)[ \t]+\\{[ \t]+e[ \t]+=[ \t]+\\(tmpnam\\(b\\)[ \t]+==[ \t]+NULL\\);[ \t]+\\}")

    set(NEW_MULTI_MACRO [[
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#include <io.h>
#include <process.h>
#define lua_tmpnam(b,e) { \
  char *tmp_dir = getenv("TMP"); \
  if (!tmp_dir) tmp_dir = getenv("TEMP"); \
  if (!tmp_dir) tmp_dir = "."; \
  snprintf(b, LUA_TMPNAMBUFSIZE, "%s\\lua_XXXXXX", tmp_dir); \
  e = (_mktemp_s(b, strlen(b) + 1) != 0); \
}
#else
#include <unistd.h>
#define lua_tmpnam(b,e) { \
  char *tmp_dir = getenv("TMPDIR"); \
  if (!tmp_dir) tmp_dir = "/tmp"; \
  snprintf(b, LUA_TMPNAMBUFSIZE, "%s/lua_XXXXXX", tmp_dir); \
  int fd = mkstemp(b); \
  if (fd != -1) { \
    close(fd); \
    e = 0; \
  } else { \
    e = 1; \
  } \
}
#endif]])

    if(FILE_CONTENTS MATCHES "tmpnam\\(b\\)")
        string(REGEX REPLACE "${OLD_MACRO_REGEX}" "${NEW_MULTI_MACRO}" MODIFIED_CONTENTS "${FILE_CONTENTS}")
        file(WRITE "${LOSLIB_PATH}" "${MODIFIED_CONTENTS}")
    endif()
endif()

file(GLOB_RECURSE SOL2_PATCH_HEADERS external/remake2d/sol2/*.hpp)
foreach(SOL2_HPP ${SOL2_PATCH_HEADERS})
    file(READ ${SOL2_HPP} SOL2_CONTENT)
    string(REPLACE "#include <sol/" "#include <remake2d/sol2/" SOL2_CONTENT "${SOL2_CONTENT}")
    string(REPLACE "__has_include(<lua/" "__has_include(<remake2d/lua/" SOL2_CONTENT "${SOL2_CONTENT}")
    string(REPLACE "#include <lua/" "#include <remake2d/lua/" SOL2_CONTENT "${SOL2_CONTENT}")
    file(WRITE ${SOL2_HPP} "${SOL2_CONTENT}")
endforeach()

file(GLOB_RECURSE LUA_SRC external/remake2d/lua/*.c)
add_library(lualib OBJECT ${LUA_SRC})

target_compile_options(lualib PRIVATE -Wno-unused-parameter)
target_include_directories(lualib PRIVATE external/remake2d)
set_target_properties(lualib PROPERTIES LINKER_LANGUAGE C)

file(GLOB_RECURSE REMAKE_SCRIPT_SRC src/remake2d/*.cpp)
file(GLOB_RECURSE REMAKE_SCRIPT_CONFIG_SRC src/remake2d/config/*.cpp)

if(BUILD_SHARED_LIBS)
    add_library(remakeScript SHARED
		$<TARGET_OBJECTS:lualib>
		${REMAKE_SCRIPT_SRC}
		${REMAKE_SCRIPT_CONFIG_SRC}
	)
else()
	add_library(remakeScript STATIC
		$<TARGET_OBJECTS:lualib>
		${REMAKE_SCRIPT_SRC}
		${REMAKE_SCRIPT_CONFIG_SRC}
	)
endif()



target_include_directories(
    remakeScript
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/external
)

target_compile_definitions(
    remakeScript
    PUBLIC
        REMAKE2D_ADDON_SCRIPT
    PRIVATE
        SDL_MAIN_HANDLED
)

target_link_libraries(
	remakeScript
	PUBLIC
		remake2d
)

if(BUILD_SHARED_LIBS AND MSVC)
  set_target_properties(remakeScript PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if(WIN32)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(remakeScript PRIVATE -Wa,-mbig-obj)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(remakeScript PRIVATE /bigobj)
    endif()
endif()

install(
	TARGETS remakeScript
	EXPORT remakeScriptTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    EXPORT remakeScriptTargets
    FILE remakeScriptTargets.cmake
    NAMESPACE remake2d::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/remake2d
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/remake2d
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/remake2d/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/remake2d
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
    PATTERN "*.c" EXCLUDE
    PATTERN "*.cpp" EXCLUDE
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/remake2d/template
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/remake2d
    FILES_MATCHING PATTERN "*.tpp"
)