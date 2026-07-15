# generate_def.cmake
#
# Usage:
#   cmake -DNM_EXECUTABLE=<path-to-nm> -DOBJECT_FILES="a.obj;b.obj;..." \
#         -DOUTPUT_FILE=<path> -P generate_def.cmake
#
# Generates a Windows module-definition (.def) file listing only the
# externally-linked ("global") symbols defined in the given object files.
#
# Why this exists: on MinGW, remake2d's own sources (and vendored box2d/lua,
# baked in via remake2d_thirdparty) never mark anything with
# __declspec(dllexport) -- box2d/lua only do so when building with MSVC.
# With nothing explicitly marked for export, GNU ld's default fallback is to
# auto-export every global symbol it can see, which -- combined with sol2's
# heavily templated Lua bindings compiled directly into remake2d's own
# objects -- overflows the PE format's 16-bit export ordinal limit (65535),
# especially in unoptimized Debug builds where nothing gets inlined away.
#
# Supplying an explicit .def file to the linker disables that auto-export
# fallback entirely (per the binutils PE/WIN32 documentation), without
# requiring any source changes. This script builds that .def file
# automatically from remake2d's own compiled objects only (not
# box2d/lua/sol2 internals), so only remake2d's real public API ends up in
# the DLL's export table.

if(NOT NM_EXECUTABLE)
    message(FATAL_ERROR "generate_def.cmake: NM_EXECUTABLE not set")
endif()
if(NOT OBJECT_FILES)
    message(FATAL_ERROR "generate_def.cmake: OBJECT_FILES not set")
endif()
if(NOT OUTPUT_FILE)
    message(FATAL_ERROR "generate_def.cmake: OUTPUT_FILE not set")
endif()

execute_process(
    COMMAND ${NM_EXECUTABLE} --defined-only -g ${OBJECT_FILES}
    OUTPUT_VARIABLE NM_OUTPUT
    RESULT_VARIABLE NM_RESULT
    ERROR_VARIABLE NM_ERROR
)

if(NOT NM_RESULT EQUAL 0)
    message(FATAL_ERROR "generate_def.cmake: '${NM_EXECUTABLE}' failed (exit ${NM_RESULT}): ${NM_ERROR}")
endif()

string(REPLACE "\n" ";" NM_LINES "${NM_OUTPUT}")
string(REPLACE "\r" "" NM_LINES "${NM_LINES}")

set(SYMBOLS "")
foreach(line IN LISTS NM_LINES)
    # A real symbol line looks like: "0000000000000000 T _ZN3rmk6SystemC1Ev"
    # nm also prints "path/to/file.obj:" header lines and blank lines when
    # given multiple object files at once; only lines below match a symbol.
    if(line MATCHES "^[0-9a-fA-F]+ ([A-Za-z]) (.+)$")
        set(_type "${CMAKE_MATCH_1}")
        set(_name "${CMAKE_MATCH_2}")
        # nm convention: UPPERCASE type letter = external/global linkage.
        # lowercase = local (static) symbol -- must NOT be exported, and
        # normally wouldn't even show up here since these are usually not
        # visible with -g, but we double check to be safe.
        if(_type MATCHES "^[A-Z]$")
            list(APPEND SYMBOLS "${_name}")
        endif()
    endif()
endforeach()

if(NOT SYMBOLS)
    message(FATAL_ERROR "generate_def.cmake: no exportable symbols found -- refusing to write an empty .def file")
endif()

list(REMOVE_DUPLICATES SYMBOLS)
list(SORT SYMBOLS)

set(DEF_CONTENT "EXPORTS\n")
foreach(sym IN LISTS SYMBOLS)
    string(APPEND DEF_CONTENT "    ${sym}\n")
endforeach()

file(WRITE "${OUTPUT_FILE}" "${DEF_CONTENT}")

list(LENGTH SYMBOLS SYMBOL_COUNT)
message(STATUS "generate_def.cmake: wrote ${SYMBOL_COUNT} exported symbols to ${OUTPUT_FILE}")
