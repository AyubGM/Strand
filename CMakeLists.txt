cmake_minimum_required(VERSION 3.80)
project(Strand)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------------------------------------------------------------
# Global Settings
# ------------------------------------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/bin)
endforeach()

# Visual Studio startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Bridges)

# Multi-processor compilation for MSVC
if(MSVC)
    add_compile_options(/MP)
    add_compile_options(/utf-8)
endif()

# Platform
if(WIN32)
    add_definitions(-DSD_PLATFORM_WINDOWS)
endif()

# ------------------------------------------------------------------------------
# Common paths (used by all subdirectories)
# ------------------------------------------------------------------------------
set(SD_ROOT ${CMAKE_SOURCE_DIR})
set(SD_VENDOR_DIR ${CMAKE_SOURCE_DIR}/Strand/vendor)
set(SD_SRC_DIR ${CMAKE_SOURCE_DIR}/Strand/src)

# ------------------------------------------------------------------------------
# Vulkan SDK
# ------------------------------------------------------------------------------
if(NOT DEFINED ENV{VULKAN_SDK})
    message(FATAL_ERROR "VULKAN_SDK environment variable is not set!")
endif()
set(VULKAN_SDK $ENV{VULKAN_SDK})

# ------------------------------------------------------------------------------
# Include shared module for app setup
# ------------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(Common)


# ------------------------------------------------------------------------------
# Dependencies (bundled third-party libraries)
# ------------------------------------------------------------------------------
add_subdirectory(Strand/vendor/GLFW)
add_subdirectory(Strand/vendor/Glad)
set(BOX2D_BUILD_TESTBED OFF CACHE INTERNAL "")
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE INTERNAL "")
set(BOX2D_BUILD_DOCS OFF CACHE INTERNAL "")
add_subdirectory(Strand/vendor/Box2D)
add_subdirectory(Strand/vendor/googletest)
add_subdirectory(Strand/vendor/ImGuiColorTextEdit)
add_subdirectory(Strand/vendor/imgui)
add_subdirectory(Strand/vendor/yaml-cpp)
add_subdirectory(Strand/vendor/assimp)

set_target_properties(box2d PROPERTIES FOLDER "Dependencies")
#set_target_properties(box2d PROPERTIES
   # ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bin"
  #  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bin"
 #   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bin"
#)
set_target_properties(gtest PROPERTIES FOLDER "Dependencies")
set_target_properties(gtest_main PROPERTIES FOLDER "Dependencies")
set_target_properties(gmock PROPERTIES FOLDER "Dependencies")
set_target_properties(gmock_main PROPERTIES FOLDER "Dependencies")
set_target_properties(ImGuiColorTextEdit PROPERTIES FOLDER "Dependencies")
set_target_properties(glfw PROPERTIES FOLDER "Dependencies")
set_target_properties(Glad PROPERTIES FOLDER "Dependencies")
set_target_properties(ImGui PROPERTIES FOLDER "Dependencies")
set_target_properties(yaml-cpp PROPERTIES FOLDER "Dependencies")
set_target_properties(assimp PROPERTIES FOLDER "Dependencies")

# ------------------------------------------------------------------------------
# Engine & Applications
# ------------------------------------------------------------------------------
add_subdirectory(Strand)
add_subdirectory(Sandbox)
add_subdirectory(StrandLabs)
add_subdirectory(Bridges)
add_subdirectory(Strand-ScriptCore)