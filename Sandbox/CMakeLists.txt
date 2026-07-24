# Sandbox Application
# ==============================================================================

file(GLOB_RECURSE SANDBOX_SOURCES
    src/*.h
    src/*.cpp
)

add_executable(Sandbox ${SANDBOX_SOURCES})
strand_app_setup(Sandbox)