# Strand Engine Core Library
# ==============================================================================

# ------------------------------------------------------------------------------
# Source files
# ------------------------------------------------------------------------------
file(GLOB_RECURSE STRAND_SOURCES
    src/*.h
    src/*.cpp
    vendor/stb_image/*.h
    vendor/stb_image/*.cpp
    vendor/glm/glm/*.hpp
    vendor/glm/glm/*.inl
    vendor/ImGuizmo/ImGuizmo.h
    vendor/ImGuizmo/ImGuizmo.cpp
)

add_library(Strand STATIC ${STRAND_SOURCES})

# ------------------------------------------------------------------------------
# Precompiled Headers
# ------------------------------------------------------------------------------
target_precompile_headers(Strand PRIVATE src/sdpch.h)

# Disable PCH for ImGuizmo (third-party, doesn't include sdpch.h)
set_source_files_properties(vendor/ImGuizmo/ImGuizmo.cpp PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

# ------------------------------------------------------------------------------
# Include Directories
# ------------------------------------------------------------------------------
target_include_directories(Strand PUBLIC
    # Engine source
    src

    # Third-party headers (bundled)
    vendor/spdlog/include
    vendor/GLFW/include
    vendor/Glad/include
    vendor/imgui
    vendor/glm
    vendor/Box2D/include
    vendor/googletest/googletest/include
    vendor/googletest/googlemock/include
    ${SD_ROOT}/Strand/vendor/filewatch
    ${SD_ROOT}/Strand/vendor/ImGuiColorTextEdit
    vendor/stb_image
    vendor/entt/include
    vendor/mono/include
    vendor/yaml-cpp/include
    vendor/ImGuizmo
    vendor/shaderc/include
    vendor/SPIRV-Cross
    vendor/assimp/include

    # Vulkan SDK (external)
    ${VULKAN_SDK}/Include
)

# ------------------------------------------------------------------------------
# Compile Definitions
# ------------------------------------------------------------------------------
target_compile_definitions(Strand PUBLIC
    _CRT_SECURE_NO_WARNINGS
    GLFW_INCLUDE_NONE
    YAML_CPP_STATIC_DEFINE
    IMGUI_DEFINE_MATH_OPERATORS
)

# Configuration-specific defines
target_compile_definitions(Strand PUBLIC
    $<$<CONFIG:Debug>:SD_DEBUG>
    $<$<CONFIG:Release>:SD_RELEASE>
    $<$<CONFIG:Dist>:SD_DIST>
)

# ------------------------------------------------------------------------------
# Base Links (bundled dependencies)
# ------------------------------------------------------------------------------
target_link_libraries(Strand PUBLIC
    box2d
    gtest
    glfw
    Glad
    ImGui
    ImGuiColorTextEdit
    yaml-cpp
    opengl32
)

# ------------------------------------------------------------------------------
# Windows System Libraries
# ------------------------------------------------------------------------------
if(WIN32)
    target_link_libraries(Strand PUBLIC
        Ws2_32
        Winmm
        Version
        Bcrypt
    )
endif()

# ------------------------------------------------------------------------------
# External Libraries: Vulkan SDK
# ------------------------------------------------------------------------------
# Use find_library to locate Vulkan SDK libraries properly
find_library(VULKAN_LIB
    NAMES vulkan-1
    PATHS ${VULKAN_SDK}/Lib
    NO_DEFAULT_PATH
    REQUIRED
)

find_library(SHADERC_DEBUG_LIB
    NAMES shaderc_sharedd
    PATHS ${VULKAN_SDK}/Lib
    NO_DEFAULT_PATH
)

find_library(SPIRV_CROSS_DEBUG_LIB
    NAMES spirv-cross-cored
    PATHS ${VULKAN_SDK}/Lib
    NO_DEFAULT_PATH
)

find_library(SPIRV_CROSS_GLSL_DEBUG_LIB
    NAMES spirv-cross-glsld
    PATHS ${VULKAN_SDK}/Lib
    NO_DEFAULT_PATH
)

find_library(SHADERC_RELEASE_LIB
    NAMES shaderc_shared
    PATHS ${VULKAN_SDK}/Lib
    NO_DEFAULT_PATH
)

find_library(SPIRV_CROSS_RELEASE_LIB
    NAMES spirv-cross-core
    PATHS ${VULKAN_SDK}/Lib
    NO_DEFAULT_PATH
)

find_library(SPIRV_CROSS_GLSL_RELEASE_LIB
    NAMES spirv-cross-glsl
    PATHS ${VULKAN_SDK}/Lib
    NO_DEFAULT_PATH
)

# Link Vulkan libs per configuration
target_link_libraries(Strand PUBLIC
    $<$<CONFIG:Debug>:${SHADERC_DEBUG_LIB}>
    $<$<CONFIG:Debug>:${SPIRV_CROSS_DEBUG_LIB}>
    $<$<CONFIG:Debug>:${SPIRV_CROSS_GLSL_DEBUG_LIB}>
    $<$<CONFIG:Release>:${SHADERC_RELEASE_LIB}>
    $<$<CONFIG:Release>:${SPIRV_CROSS_RELEASE_LIB}>
    $<$<CONFIG:Release>:${SPIRV_CROSS_GLSL_RELEASE_LIB}>
    $<$<CONFIG:Dist>:${SHADERC_RELEASE_LIB}>
    $<$<CONFIG:Dist>:${SPIRV_CROSS_RELEASE_LIB}>
    $<$<CONFIG:Dist>:${SPIRV_CROSS_GLSL_RELEASE_LIB}>
)

# ------------------------------------------------------------------------------
# External Libraries: Assimp
# ------------------------------------------------------------------------------
# Try to find assimp via CMake config first, fallback to find_library
find_package(assimp CONFIG QUIET)
if(assimp_FOUND)
    target_link_libraries(Strand PUBLIC assimp::assimp)
    message(STATUS "Found assimp via CMake config")
else()
    # Fallback: search for assimp library manually
    # Search in common locations relative to vendor/assimp
    find_library(ASSIMP_DEBUG_LIB
        NAMES assimp-vc143-mtd assimp-vc142-mtd assimpd assimp
        PATHS
            vendor/assimp/lib/Debug
            vendor/assimp/lib
            ${SD_ROOT}/vendor/assimp/lib/Debug
            ${SD_ROOT}/vendor/assimp/lib
        NO_DEFAULT_PATH
    )

    find_library(ASSIMP_RELEASE_LIB
        NAMES assimp-vc143-mt assimp-vc142-mt assimp
        PATHS
            vendor/assimp/lib/Release
            vendor/assimp/lib
            ${SD_ROOT}/vendor/assimp/lib/Release
            ${SD_ROOT}/vendor/assimp/lib
        NO_DEFAULT_PATH
    )

    find_library(ZLIB_DEBUG_LIB
        NAMES zlibstaticd zlibstatic
        PATHS
            vendor/assimp/contrib/zlib/Debug
            vendor/assimp/contrib/zlib
            ${SD_ROOT}/vendor/assimp/contrib/zlib/Debug
            ${SD_ROOT}/vendor/assimp/contrib/zlib
        NO_DEFAULT_PATH
    )

    if(ASSIMP_DEBUG_LIB AND ASSIMP_RELEASE_LIB)
        target_link_libraries(Strand PUBLIC
            $<$<CONFIG:Debug>:${ASSIMP_DEBUG_LIB}>
            $<$<CONFIG:Debug>:${ZLIB_DEBUG_LIB}>
            $<$<CONFIG:Release>:${ASSIMP_RELEASE_LIB}>
            $<$<CONFIG:Dist>:${ASSIMP_RELEASE_LIB}>
        )
        message(STATUS "Found assimp manually: Debug=${ASSIMP_DEBUG_LIB}, Release=${ASSIMP_RELEASE_LIB}")
    else()
        message(WARNING "Assimp not found! Set ASSIMP_ROOT or build from vendor/assimp")
    endif()
endif()

# ------------------------------------------------------------------------------
# External Libraries: Mono
# ------------------------------------------------------------------------------
find_library(MONO_DEBUG_LIB
    NAMES mono-static-sgen libmono-static-sgen
    PATHS
        vendor/mono/lib/Debug
        vendor/mono/lib
        ${SD_ROOT}/vendor/mono/lib/Debug
        ${SD_ROOT}/vendor/mono/lib
    NO_DEFAULT_PATH
)

find_library(MONO_RELEASE_LIB
    NAMES mono-static-sgen libmono-static-sgen
    PATHS
        vendor/mono/lib/Release
        vendor/mono/lib
        ${SD_ROOT}/vendor/mono/lib/Release
        ${SD_ROOT}/vendor/mono/lib
    NO_DEFAULT_PATH
)

find_library(MONO_DIST_LIB
    NAMES mono-static-sgen libmono-static-sgen
    PATHS
        vendor/mono/lib/Dist
        vendor/mono/lib
        ${SD_ROOT}/vendor/mono/lib/Dist
        ${SD_ROOT}/vendor/mono/lib
    NO_DEFAULT_PATH
)

if(MONO_DEBUG_LIB OR MONO_RELEASE_LIB)
    target_link_libraries(Strand PUBLIC
        $<$<CONFIG:Debug>:${MONO_DEBUG_LIB}>
        $<$<CONFIG:Release>:${MONO_RELEASE_LIB}>
        $<$<CONFIG:Dist>:${MONO_DIST_LIB}>
    )
    message(STATUS "Found Mono: Debug=${MONO_DEBUG_LIB}")
else()
    message(WARNING "Mono not found! Set MONO_ROOT or build from vendor/mono")
endif()

# ------------------------------------------------------------------------------
# Target Properties
# ------------------------------------------------------------------------------
set_target_properties(Strand PROPERTIES FOLDER "Engine")