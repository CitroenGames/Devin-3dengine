cmake_minimum_required(VERSION 3.15)

project(Devin-3dengine)

# Enable modern C++ features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(deps)

file(GLOB_RECURSE sources src/*.cpp src/*.h)

add_executable(Devin-3dengine ${sources})

# Include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC src
    PUBLIC ${CMAKE_SOURCE_DIR}/deps/assimp-5.4.3/include
    PUBLIC ${CMAKE_SOURCE_DIR}/deps/glfw-3.4/include
)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE glfw
    PRIVATE assimp
    PRIVATE ${OPENGL_LIBRARIES}
)
