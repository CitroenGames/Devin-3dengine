cmake_minimum_required(VERSION 3.15)

project(Devin-3dengine)

# Enable modern C++ features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add GLM
include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

# Add GLAD
add_subdirectory(deps/glad)

# Add STB
add_subdirectory(deps/stb)

add_subdirectory(deps)

file(GLOB_RECURSE sources src/*.cpp src/*.hpp)

add_executable(${PROJECT_NAME} ${sources})

# Include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC src
    PUBLIC ${CMAKE_SOURCE_DIR}/deps/assimp-5.4.3/include
    PUBLIC ${CMAKE_SOURCE_DIR}/deps/glfw-3.4/include
    PUBLIC ${CMAKE_SOURCE_DIR}/deps/glad/include
    PUBLIC ${CMAKE_SOURCE_DIR}/deps/stb
    PUBLIC ${glm_SOURCE_DIR}
)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE glfw
    PRIVATE assimp
    PRIVATE glad
    PRIVATE stb
    PRIVATE OpenGL::GL
    PRIVATE glm::glm
)
