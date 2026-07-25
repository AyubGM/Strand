# Sandbox Application
# ==============================================================================

file(GLOB_RECURSE SANDBOX_SOURCES
    src/*.h
    src/*.cpp
)

add_executable(Sandbox ${SANDBOX_SOURCES})
strand_app_setup(Sandbox)

add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Sandbox/assets"
        "$<TARGET_FILE_DIR:Sandbox>/assets"
)

#add_custom_command(TARGET Bridges POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#        "${CMAKE_SOURCE_DIR}/Bridges/Resources"
#        "$<TARGET_FILE_DIR:Bridges>/Resources"
#)