cmake_minimum_required(VERSION 3.5)

# Declare the project name and language (Glad is written in C)
project(Glad LANGUAGES C)

# Create a static library using the glad source file
add_library(Glad STATIC
    src/glad.c
)

# Set the include directories for the target
# PUBLIC means that any project linking to 'Glad' will automatically 
# get this include directory added to its search path.
target_include_directories(Glad PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Optional: Group files for Visual Studio solution explorer (matches your vcxproj layout)
source_group("Header Files\\glad" FILES include/glad/glad.h)
source_group("Header Files\\KHR"  FILES include/KHR/khrplatform.h)
source_group("Source Files"       FILES src/glad.c)