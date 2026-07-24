# Bridges Editor Application
# ==============================================================================

file(GLOB_RECURSE BRIDGES_SOURCES
    src/*.h
    src/*.cpp
)

add_executable(Bridges ${BRIDGES_SOURCES})

strand_app_setup(Bridges
    EXTRA_INCLUDES
        ${SD_ROOT}/Strand/vendor/filewatch
        ${SD_ROOT}/Strand/vendor/ImGuiColorTextEdit
        ../Strand/vendor/ImGuizmo
)

add_custom_command(TARGET Bridges POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Bridges/assets"
        "$<TARGET_FILE_DIR:Bridges>/assets"
)

add_custom_command(TARGET Bridges POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Bridges/Resources"
        "$<TARGET_FILE_DIR:Bridges>/Resources"
)