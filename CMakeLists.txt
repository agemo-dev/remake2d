cmake_minimum_required(VERSION 3.32)

project(
	remake2d
	VERSION 1.0.0
	DESCRIPTION "A cross-plateform 2D game engine"
	LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type" FORCE)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(USE_NATIVE_OPT "Enable -march=native optimizations" OFF)
option(REMAKE2D_TERMUX_DOWNLOAD_DEPENDENCIES "Download dependencies" OFF)

if(USE_NATIVE_OPT)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
endif()

if(EXISTS "/data/data/com.termux")
    set(TERMUX_BUILD ON)
    set(CMAKE_SYSTEM_NAME Linux CACHE STRING "" FORCE)
	message("Termux detected — plateform set to 'Linux'")
else()
    set(TERMUX_BUILD OFF)
endif()

include(FetchContent)

set(BUILD_SHARED_LIBS ON)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

if(TERMUX_BUILD)
    find_package(SDL2 QUIET)
    if(NOT SDL2_FOUND)
        execute_process(
            COMMAND pkg install -y x11-repo
            COMMAND pkg install -y libsdl2 libsdl2-ttf libsdl2-image libsdl2-mixer
            RESULT_VARIABLE TERMUX_INSTALL_RES
        )
        if(NOT TERMUX_INSTALL_RES EQUAL 0)
            message(FATAL_ERROR "pkg install failed")
        endif()
    endif()

    find_package(SDL2       REQUIRED)
    find_package(SDL2_ttf   REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_mixer REQUIRED)
else()
    set(SDL2_DISABLE_INSTALL   OFF CACHE BOOL "" FORCE)
    set(SDL2IMAGE_INSTALL      ON  CACHE BOOL "" FORCE)
    set(SDL2MIXER_INSTALL      ON  CACHE BOOL "" FORCE)
    set(SDL2TTF_INSTALL        ON  CACHE BOOL "" FORCE)

    set(SDL2IMAGE_VENDORED ON CACHE BOOL "" FORCE)
    set(SDL2MIXER_VENDORED ON CACHE BOOL "" FORCE)
    set(SDL2TTF_VENDORED   ON CACHE BOOL "" FORCE)

    set(USER_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
    set(BUILD_SHARED_LIBS OFF)

	set(SDL2IMAGE_DEPS_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL2MIXER_DEPS_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL2TTF_DEPS_SHARED   OFF CACHE BOOL "" FORCE)
	
    set(SDL2MIXER_WAV    ON  CACHE BOOL "" FORCE)
    set(SDL2MIXER_MP3    ON  CACHE BOOL "" FORCE)
    set(SDL2MIXER_FLAC   ON  CACHE BOOL "" FORCE)
	
    set(SDL2MIXER_OPUS   OFF CACHE BOOL "" FORCE)
    set(SDL2MIXER_MOD    OFF CACHE BOOL "" FORCE)
    set(SDL2MIXER_MIDI   OFF CACHE BOOL "" FORCE)

    set(SDL2IMAGE_PNG    ON  CACHE BOOL "" FORCE)
    set(SDL2IMAGE_JPG    ON  CACHE BOOL "" FORCE)
    set(SDL2IMAGE_WEBP   ON  CACHE BOOL "" FORCE)
    set(SDL2IMAGE_TGA    ON  CACHE BOOL "" FORCE)
    set(SDL2IMAGE_BMP    ON  CACHE BOOL "" FORCE)
	
    set(SDL2IMAGE_AVIF   OFF CACHE BOOL "" FORCE)
    set(SDL2IMAGE_GIF    OFF CACHE BOOL "" FORCE)
    set(SDL2IMAGE_LBM    OFF CACHE BOOL "" FORCE)
    set(SDL2IMAGE_PCX    OFF CACHE BOOL "" FORCE)
    set(SDL2IMAGE_PNM    OFF CACHE BOOL "" FORCE)
    set(SDL2IMAGE_SVG    OFF CACHE BOOL "" FORCE)
    set(SDL2IMAGE_TIF    OFF CACHE BOOL "" FORCE)
    set(SDL2IMAGE_XCF    OFF CACHE BOOL "" FORCE)
    set(SDL2IMAGE_XPM    OFF CACHE BOOL "" FORCE)
    set(SDL2IMAGE_XV     OFF CACHE BOOL "" FORCE)
    set(SDL2IMAGE_QOI    OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.11
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    
    FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.8.6
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    
    FetchContent_Declare(
        SDL2_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG release-2.8.2
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    
    FetchContent_Declare(
        SDL2_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG release-2.24.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    
    FetchContent_MakeAvailable(SDL2 SDL2_image SDL2_mixer SDL2_ttf)
    
    set(BUILD_SHARED_LIBS ${USER_BUILD_SHARED_LIBS})

    if(NOT TARGET SDL2::SDL2)
        if(TARGET SDL2)
            add_library(SDL2::SDL2 ALIAS SDL2)
        elseif(TARGET SDL2-static)
            add_library(SDL2::SDL2 ALIAS SDL2-static)
        endif()
    endif()

    if(NOT TARGET SDL2_image::SDL2_image AND TARGET SDL2_image)
        add_library(SDL2_image::SDL2_image ALIAS SDL2_image)
    endif()

    if(NOT TARGET SDL2_mixer::SDL2_mixer AND TARGET SDL2_mixer)
        add_library(SDL2_mixer::SDL2_mixer ALIAS SDL2_mixer)
    endif()

    if(NOT TARGET SDL2_ttf::SDL2_ttf AND TARGET SDL2_ttf)
        add_library(SDL2_ttf::SDL2_ttf ALIAS SDL2_ttf)
    endif()

    set(SDL2_SHIM_DIR ${CMAKE_BINARY_DIR}/sdl2_include_shim)
    file(MAKE_DIRECTORY ${SDL2_SHIM_DIR}/SDL2)

    function(remake2d_shim_header SRC_DIR HEADER_NAME)
        file(GLOB_RECURSE FOUND "${SRC_DIR}/${HEADER_NAME}")
        if(FOUND)
            list(GET FOUND 0 HDR_PATH)
            set(DEST ${SDL2_SHIM_DIR}/SDL2/${HEADER_NAME})
            if(NOT EXISTS ${DEST})
                file(COPY_FILE ${HDR_PATH} ${DEST})
            endif()
        else()
            message(WARNING "remake2d: ${HEADER_NAME} not found under ${SRC_DIR}")
        endif()
    endfunction()

    remake2d_shim_header(${sdl2_ttf_SOURCE_DIR}   SDL_ttf.h)
    remake2d_shim_header(${sdl2_image_SOURCE_DIR} SDL_image.h)
    remake2d_shim_header(${sdl2_mixer_SOURCE_DIR} SDL_mixer.h)

    file(GLOB SHIM_CHECK "${SDL2_SHIM_DIR}/SDL2/*.h")
    list(LENGTH SHIM_CHECK SHIM_CHECK_COUNT)
    message(STATUS "SDL2 shim headers copied: ${SHIM_CHECK_COUNT}")
endif()

set(BOX2D_BUILD_TESTBED    OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory(external/remake2d/box2d)

file(GLOB_RECURSE LUA_SRC external/remake2d/lua/*.c)

add_library(lualib OBJECT ${LUA_SRC})
target_compile_options(lualib PRIVATE -Wno-unused-parameter)
target_include_directories(lualib PRIVATE ${CMAKE_SOURCE_DIR}/external/remake2d)
set_target_properties(lualib PROPERTIES LINKER_LANGUAGE C)

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

file(GLOB_RECURSE REMAKE2D_SRC src/remake2d/*.cpp)
file(GLOB_RECURSE REMAKE_SCRIPT_SRC src/remake2d/config/solstat/*.cpp)

list(REMOVE_ITEM REMAKE2D_SRC ${REMAKE_SCRIPT_SRC})

# remakeScript : remake 2D - Scripting .
# compile scripting files on static lib .
# because they generates so symbol for Windows compilers
add_library(remakeScript STATIC ${REMAKE_SCRIPT_SRC})

target_include_directories(
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external
        $<$<NOT:$<BOOL:${TERMUX_BUILD}>>:${SDL2_SHIM_DIR}>
)

target_compile_definitions(
    remakeScript
    PUBLIC
        SDL_MAIN_HANDLED
)

target_link_libraries(
    remakeScript
    PRIVATE
        SDL2::SDL2
        SDL2_ttf::SDL2_ttf
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer
)

if(WIN32)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(remakeScript PRIVATE -Wa,-mbig-obj)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(remakeScript PRIVATE /bigobj)
    endif()
endif()

# remakeEXT : remake 2D - External .
# link vendored dependencies on static lib .
add_library(remakeEXT STATIC
    $<TARGET_OBJECTS:box2d>
    $<TARGET_OBJECTS:lualib>
)
set_target_properties(remakeEXT PROPERTIES LINKER_LANGUAGE CXX)

if(BUILD_SHARED_LIBS)
    add_library(
        remake2d SHARED
        ${REMAKE2D_SRC}
    )
else()
    add_library(
        remake2d STATIC
        ${REMAKE2D_SRC}
    )
endif()

target_include_directories(
    remake2d
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external
        $<$<NOT:$<BOOL:${TERMUX_BUILD}>>:${SDL2_SHIM_DIR}>
)

if(WIN32)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(remake2d PRIVATE -Wa,-mbig-obj)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(remake2d PRIVATE /bigobj)
    endif()
endif()

target_include_directories(
    remake2d
    INTERFACE
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

target_compile_definitions(
    remake2d
    PUBLIC
        SDL_MAIN_HANDLED
)

target_link_libraries(
    remake2d
    PRIVATE
        remakeEXT
		remakeScript
        SDL2::SDL2
        SDL2_ttf::SDL2_ttf
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer
		$<$<CXX_COMPILER_ID:GNU>:atomic>
        ${CMAKE_DL_LIBS}
)

if(WIN32)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_options(remake2d PRIVATE "-Wl,--exclude-libs,ALL")
    endif()
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/remake2dConfig.cmake.in
    ${CMAKE_BINARY_DIR}/remake2dConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/remake2d
)

install(TARGETS remake2d
    EXPORT remake2dTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT remake2dTargets
    FILE remake2dTargets.cmake
    NAMESPACE remake2d::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/remake2d
)

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