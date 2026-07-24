# StrandLabs Test Application
# ==============================================================================

file(GLOB_RECURSE STRANDLABS_SOURCES
    src/*.h
    src/*.cpp
)

add_executable(StrandLabs ${STRANDLABS_SOURCES})

strand_app_setup(StrandLabs
    EXTRA_INCLUDES
        ../Strand/vendor/googletest/googletest/include
        ../Strand/vendor/googletest/googlemock/include
        ${SD_ROOT}/Strand/vendor/filewatch
        ${SD_ROOT}/Strand/vendor/ImGuiColorTextEdit
        ../Strand/vendor/ImGuizmo
)