# Strand-ScriptCore C# Assembly
# ==============================================================================
# This CMakeLists.txt does NOT use CMake's built-in C# language support.
# Instead, it invokes MSBuild/dotnet directly on the existing .csproj file.
# This is far more reliable for .NET Framework 4.7.2 projects.
# ==============================================================================

# ------------------------------------------------------------------------------
# Find the build tool
# ------------------------------------------------------------------------------
# Prefer MSBuild (comes with Visual Studio / Build Tools)
# Fallback to dotnet CLI
find_program(MSBUILD_EXECUTABLE
    NAMES MSBuild.exe msbuild
    PATHS
        "C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin"
        "C:/Program Files/Microsoft Visual Studio/2022/Professional/MSBuild/Current/Bin"
        "C:/Program Files/Microsoft Visual Studio/2022/Enterprise/MSBuild/Current/Bin"
        "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/MSBuild/Current/Bin"
        "C:/Program Files (x86)/Microsoft Visual Studio/2019/Professional/MSBuild/Current/Bin"
        "C:/Program Files (x86)/Microsoft Visual Studio/2019/Enterprise/MSBuild/Current/Bin"
        "C:/Program Files (x86)/MSBuild/14.0/Bin"
    DOC "MSBuild executable"
)

find_program(DOTNET_EXECUTABLE
    NAMES dotnet
    DOC "dotnet CLI executable"
)

if(MSBUILD_EXECUTABLE)
    set(SCRIPTCORE_BUILD_TOOL ${MSBUILD_EXECUTABLE})
    set(SCRIPTCORE_BUILD_ARGS "/m" "/verbosity:minimal")
    message(STATUS "Strand-ScriptCore: Using MSBuild: ${MSBUILD_EXECUTABLE}")
elseif(DOTNET_EXECUTABLE)
    set(SCRIPTCORE_BUILD_TOOL ${DOTNET_EXECUTABLE})
    set(SCRIPTCORE_BUILD_ARGS "build")
    message(STATUS "Strand-ScriptCore: Using dotnet CLI: ${DOTNET_EXECUTABLE}")
else()
    message(FATAL_ERROR 
        "Strand-ScriptCore: Neither MSBuild nor dotnet CLI found! "
        "Install Visual Studio Build Tools or .NET SDK."
    )
endif()