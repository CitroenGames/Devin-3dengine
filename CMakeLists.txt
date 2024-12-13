cmake_minimum_required(VERSION 3.15)

project(Devin-3dengine)

add_subdirectory(deps)

file(GLOB_RECURSE sources src/*.cpp src/*.h)

add_executable(Devin-3dengine ${sources})

target_link_directories(${PROJECT_NAME} PUBLIC src)

#find opengl 
find_library(OPENGL_LIBRARY OpenGL)
target_link_libraries(${PROJECT_NAME} glfw assimp)