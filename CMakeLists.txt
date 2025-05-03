cmake_minimum_required(VERSION 3.10)
project(ModelPanel)

# C++ standard
set(CMAKE_CXX_STANDARD 17)

# Path
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_SOURCE_DIR}/include)

# Include header files
include_directories(
    ${LIB_DIR}/glfw-3.4/include
    ${LIB_DIR}/glew-2.1.0/include
    ${LIB_DIR}/eigen
    ${LIB_DIR}/imgui
    ${LIB_DIR}/glm
    ${INC_DIR}
)

# Link libraries
link_directories(
    ${LIB_DIR}/glew-2.1.0/lib/Release/x64
    ${LIB_DIR}/glfw-3.4/build/src
)

# File *.cpp
add_executable(ModelPanel
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/MainPage.cpp
    ${LIB_DIR}/imgui/imgui.cpp
    ${LIB_DIR}/imgui/imgui_draw.cpp
    ${LIB_DIR}/imgui/imgui_tables.cpp
    ${LIB_DIR}/imgui/imgui_widgets.cpp
)

# Link libraries
target_link_libraries(ModelPanel
    glfw3
    glew32
    opengl32
    glu32
)


# cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
# cmake --build .