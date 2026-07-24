cmake_minimum_required(VERSION 3.8)
project(ImGuiColorTextEdit)

# Sources
set(IMGUI_COLOR_TEXT_EDIT_SOURCES
	TextEditor.cpp
)

set(IMGUI_COLOR_TEXT_EDIT_HEADERS
	TextEditor.h
)

add_library(ImGuiColorTextEdit STATIC
	${IMGUI_COLOR_TEXT_EDIT_SOURCES}
	${IMGUI_COLOR_TEXT_EDIT_HEADERS}
)

target_include_directories(ImGuiColorTextEdit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(ImGuiColorTextEdit PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF)

# If your project uses ImGui, uncomment the following line to link against it
# target_link_libraries(ImGuiColorTextEdit PUBLIC ImGui)

# Place under the Dependencies folder in IDEs that support it
set_target_properties(ImGuiColorTextEdit PROPERTIES FOLDER "Dependencies")
