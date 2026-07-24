cmake_minimum_required(VERSION 3.15)
project(ImGui LANGUAGES CXX)

# Create the static library and list all source files
add_library(ImGui STATIC
    imconfig.h
    imgui.h
    imgui.cpp
    imgui_draw.cpp
    imgui_internal.h
    imgui_tables.cpp
    imgui_widgets.cpp
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
    imgui_demo.cpp
)

# Specify C++17 dialect (applies to Windows and Linux automatically)
target_compile_features(ImGui PUBLIC cxx_std_17)

# Make sure projects linking to ImGui can find its headers
target_include_directories(ImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# -------------------------------------------------------------------
# Platform Specific Settings (Filters)
# -------------------------------------------------------------------

if (WIN32)
    # defines { "IMGUI_API=__declspec(dllexport)" }
    # Note: dllexport is highly unusual for a STATIC library. 
    # Usually, this is only used for SHARED libraries (DLLs). 
    target_compile_definitions(ImGui PRIVATE "IMGUI_API=__declspec(dllexport)")

    # staticruntime "on"
    # This links the MSVC static runtime (/MT for Release, /MTd for Debug)
    set_property(TARGET ImGui PROPERTY 
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )
endif()

if (UNIX AND NOT APPLE) # system:linux filter
    # pic "On"
    set_property(TARGET ImGui PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

# -------------------------------------------------------------------
# Output Directories (Optional in CMake)
# -------------------------------------------------------------------
# CMake natively handles intermediate objects (objdir) in the build tree. 
# If you want to force the final .lib/.a output to a specific bin/ directory:
#
# set_target_properties(ImGui PROPERTIES 
#     ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/ImGui"
# )