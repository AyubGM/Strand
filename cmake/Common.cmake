# Common.cmake
# Shared settings for all Strand Engine applications
# Include this in each app's CMakeLists.txt

# ------------------------------------------------------------------------------
# Common include directories for apps that link against Strand
# ------------------------------------------------------------------------------
set(STRAND_APP_INCLUDES
    ../Strand/vendor/spdlog/include
    ../Strand/vendor/Box2D/include
    ../Strand/src
    ../Strand/vendor
    ../Strand/vendor/glm
    ../Strand/vendor/entt/include
    ../Strand/vendor/assimp/include
)

# ------------------------------------------------------------------------------
# Common compile definitions for apps
# ------------------------------------------------------------------------------
set(STRAND_APP_DEFS
    $<$<CONFIG:Debug>:SD_DEBUG>
    $<$<CONFIG:Release>:SD_RELEASE>
    $<$<CONFIG:Dist>:SD_DIST>
)

# ------------------------------------------------------------------------------
# Common link libraries for apps
# ------------------------------------------------------------------------------
set(STRAND_APP_LIBS
    box2d
    glfw
    Glad
    ImGui
    yaml-cpp
    assimp
    Strand
)

# ------------------------------------------------------------------------------
# Function: strand_app_setup
# Sets up a standard Strand Engine application target
# Usage: strand_app_setup(TargetName [EXTRA_INCLUDES dir1 dir2 ...] [EXTRA_LIBS lib1 lib2 ...])
# ------------------------------------------------------------------------------
function(strand_app_setup TARGET_NAME)
    cmake_parse_arguments(ARG "" "" "EXTRA_INCLUDES;EXTRA_LIBS" ${ARGN})

    # Includes
    target_include_directories(${TARGET_NAME} PRIVATE ${STRAND_APP_INCLUDES})
    if(ARG_EXTRA_INCLUDES)
        target_include_directories(${TARGET_NAME} PRIVATE ${ARG_EXTRA_INCLUDES})
    endif()

    # Definitions
    target_compile_definitions(${TARGET_NAME} PRIVATE ${STRAND_APP_DEFS})

    # Links
    target_link_libraries(${TARGET_NAME} PRIVATE ${STRAND_APP_LIBS})
    # Ensure the linker can find built dependency libraries (e.g., box2d) by
    # adding the output directory of the box2d target to the link search path.
    # Use generator expression so it works for multi-config builds.
    #target_link_directories(${TARGET_NAME} PRIVATE $<TARGET_FILE_DIR:box2d>)
    if(ARG_EXTRA_LIBS)
        target_link_libraries(${TARGET_NAME} PRIVATE ${ARG_EXTRA_LIBS})
    endif()

    # Folder
    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "Applications")

    # Post-build: copy Vulkan debug DLLs (only for Debug config)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SD_ROOT}/Strand/vendor/VulkanSDK/Bin $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMENT "Copying Vulkan debug DLLs for ${TARGET_NAME}..."
    )
endfunction()
