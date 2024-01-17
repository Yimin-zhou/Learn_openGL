cmake_minimum_required(VERSION 3.10)
project(OpenGL_Render VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

# Add subdirectories
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glew)

# Include directories
include_directories(vendor/glfw/include)
include_directories(vendor/glew/include)
include_directories(vendor/glm)
include_directories(src)

# OpenGL package
find_package(OpenGL REQUIRED)

# Core and utility source files
set(CORE_SOURCES
    "src/core/window.cpp" 
    "src/core/window.h" 
    "src/core/renderer.cpp" 
    "src/core/renderer.h"
    "src/core/application.cpp" 
    "src/core/application.h"
)

set(UTIL_SOURCES
    "src/util/shader.h"
    "src/util/shader.cpp"
 "src/util/config.h")

# Add executable
add_executable(OpenGL_Render 
    "src/main.cpp"
    ${CORE_SOURCES} 
    ${UTIL_SOURCES}
)

# Copy shader files
set(SHADER_SOURCE_DIR "${CMAKE_SOURCE_DIR}/shader")
set(SHADER_DEST_DIR "${CMAKE_BINARY_DIR}/shader")
file(MAKE_DIRECTORY ${SHADER_DEST_DIR})
add_custom_command(TARGET OpenGL_Render PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${SHADER_SOURCE_DIR} ${SHADER_DEST_DIR})

# Link libraries
target_link_libraries(OpenGL_Render OpenGL::GL glfw libglew_static) 
