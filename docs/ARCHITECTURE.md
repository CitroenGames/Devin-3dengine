# 3D Engine Architecture

## Current Structure
- `src/main.cpp`: Basic OpenGL/GLFW window setup
- CMake build system with GLFW, OpenGL, and Assimp dependencies

## Required Components

### Core Engine
- `src/core/shader.hpp/cpp`: Shader loading and management
- `src/core/mesh.hpp/cpp`: Mesh data structures and rendering
- `src/core/model.hpp/cpp`: Assimp model loading and management
- `src/core/camera.hpp/cpp`: Camera system for 3D navigation

### Rendering
- `src/rendering/renderer.hpp/cpp`: Main rendering system
- `src/rendering/shadow_map.hpp/cpp`: Shadow mapping implementation
- `src/rendering/light.hpp/cpp`: Light source management

### Resources
- `src/resources/resource_manager.hpp/cpp`: Resource loading and caching
- `assets/shaders/`: Directory for GLSL shader files
  - `basic.vert/frag`: Basic model rendering
  - `depth.vert/frag`: Shadow map generation
  - `shadow.vert/frag`: Shadow mapping implementation

## Implementation Plan

1. Core Setup
   - Add GLAD/GLEW for OpenGL function loading
   - Implement basic shader system
   - Set up camera system

2. Model Loading
   - Implement mesh class for vertex data
   - Create model loader using Assimp
   - Add basic model rendering

3. Shadow Mapping
   - Implement shadow map FBO and depth texture
   - Create shadow mapping shaders
   - Add light source management
   - Integrate shadow rendering into main pipeline

4. Resource Management
   - Implement resource manager for models/textures
   - Add shader/texture caching
   - Setup proper asset organization

## Dependencies
- GLFW 3.4: Window management
- OpenGL 3.3: Graphics API
- Assimp 5.4.3: 3D model loading
- GLAD/GLEW (to be added): OpenGL function loading
