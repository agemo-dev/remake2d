cmake_minimum_required(VERSION 3.32)
project(REMAKE2D LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type" FORCE)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

option(USE_NATIVE_OPT "Enable -march=native optimizations" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

if(USE_NATIVE_OPT)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
endif()

# ---- Termux detection ----
if(EXISTS "/data/data/com.termux")
    set(TERMUX_BUILD ON)
    set(CMAKE_SYSTEM_NAME Linux CACHE STRING "" FORCE)
else()
    set(TERMUX_BUILD OFF)
endif()

include(FetchContent)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# ---- SDL2 ----
if(TERMUX_BUILD)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    find_package(SDL2_ttf REQUIRED)
else()
    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.28.5
    )
    FetchContent_MakeAvailable(SDL2)

    FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.8.2
    )
    FetchContent_MakeAvailable(SDL2_image)

    FetchContent_Declare(
        SDL2_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG release-2.8.0
    )
    FetchContent_MakeAvailable(SDL2_mixer)

    FetchContent_Declare(
        SDL2_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG release-2.20.2
    )
    FetchContent_MakeAvailable(SDL2_ttf)
endif()

# ---- Box2D (OBJET) ----
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/remake2d/box2d)

# ---- Lua (OBJECT) ----
file(GLOB_RECURSE LUA_SRC external/remake2d/lua/*.c)
add_library(lualib OBJECT ${LUA_SRC})
target_compile_options(lualib PRIVATE -Wno-unused-parameter)
target_include_directories(lualib PRIVATE ${CMAKE_SOURCE_DIR}/external/remake2d)
set_target_properties(lualib PROPERTIES LINKER_LANGUAGE C)

# ---- Patch of external includes ----
file(GLOB_RECURSE SOL2_PATCH_HEADERS ${CMAKE_SOURCE_DIR}/external/remake2d/sol2/*.hpp)
foreach(SOL2_HPP ${SOL2_PATCH_HEADERS})
    file(READ ${SOL2_HPP} SOL2_CONTENT)
    string(REPLACE "#include <sol/" "#include <remake2d/sol2/" SOL2_CONTENT "${SOL2_CONTENT}")
    string(REPLACE "__has_include(<lua/" "__has_include(<remake2d/lua/" SOL2_CONTENT "${SOL2_CONTENT}")
    string(REPLACE "#include <lua/" "#include <remake2d/lua/" SOL2_CONTENT "${SOL2_CONTENT}")
    file(WRITE ${SOL2_HPP} "${SOL2_CONTENT}")
endforeach()

if(TERMUX_BUILD)
    file(READ ${CMAKE_SOURCE_DIR}/external/remake2d/box2d/src/core.h B2_CORE_CONTENT)
    string(REPLACE "#error Unsupported CPU" 
        "#define B2_CPU_ARM"
        B2_CORE_CONTENT "${B2_CORE_CONTENT}")
    file(WRITE ${CMAKE_SOURCE_DIR}/external/remake2d/box2d/src/core.h "${B2_CORE_CONTENT}")

    file(READ ${CMAKE_SOURCE_DIR}/external/remake2d/box2d/src/allocate.c B2_ALLOC_CONTENT)
    string(REPLACE
        "void* ptr = aligned_alloc( B2_ALIGNMENT, size32 );"
        "void* ptr = NULL; posix_memalign(&ptr, B2_ALIGNMENT, size32);"
        B2_ALLOC_CONTENT "${B2_ALLOC_CONTENT}")
    file(WRITE ${CMAKE_SOURCE_DIR}/external/remake2d/box2d/src/allocate.c "${B2_ALLOC_CONTENT}")
endif()

# ---- remake2d ----
file(GLOB_RECURSE REMAKE2D_SRC src/remake2d/*.cpp)
add_library(
    remake2d STATIC
    ${REMAKE2D_SRC}
    $<TARGET_OBJECTS:lualib>
    $<TARGET_OBJECTS:box2d>
)

target_include_directories(
    remake2d
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external
)

target_link_libraries(
    remake2d
    PUBLIC
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer
        SDL2_ttf::SDL_ttf
        pthread dl
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/remake2dConfig.cmake.in
    ${CMAKE_BINARY_DIR}/remake2dConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/remake2d
)

if(BUILD_SHARED_LIBS)
    install(
        TARGETS remake2d
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
else()
    install(
        TARGETS remake2d
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()

install(
    FILES ${CMAKE_BINARY_DIR}/remake2dConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/remake2d
)

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/remake2d
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/external/remake2d/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/remake2d
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
    PATTERN "*.c" EXCLUDE
    PATTERN "*.cpp" EXCLUDE
)

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/src/remake2d/template
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/remake2d
    FILES_MATCHING PATTERN "*.tpp"
)