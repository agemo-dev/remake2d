function(remake_copy_dll target)
    get_target_property(target_type ${target} TYPE)
    if(target_type STREQUAL "EXECUTABLE")
        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
            COMMAND_EXPAND_LISTS
        )
    endif()
endfunction()

function(remake_get_all_targets var dir)
    get_property(subdirs DIRECTORY ${dir} PROPERTY SUBDIRECTORIES)
    foreach(subdir ${subdirs})
        remake_get_all_targets(${var} ${subdir})
    endforeach()
    get_property(current_targets DIRECTORY ${dir} PROPERTY BUILDSYSTEM_TARGETS)
    
    set(local_targets ${${var}} ${current_targets})
    set(${var} ${local_targets} PARENT_SCOPE)
endfunction()

function(get_all_targets var)
    set(targets)
    remake_get_all_targets(targets ${CMAKE_SOURCE_DIR})
    set(${var} ${targets} PARENT_SCOPE)
endfunction()

function(remake_apply_copy_dll)
    get_all_targets(all_targets)
    foreach(tgt IN LISTS all_targets)
        remake_copy_dll(${tgt})
    endforeach()
endfunction()

cmake_language(
    DEFER
    DIRECTORY ${CMAKE_SOURCE_DIR}
    CALL remake_apply_copy_dll
)