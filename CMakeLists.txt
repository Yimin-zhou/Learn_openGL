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
include_directories(vendor/tinyobjloader)
include_directories(vendor/stb_image)
include_directories(src)

# OpenGL package
find_package(OpenGL 4.3 REQUIRED)

# Core and utility source files
set(CORE_SOURCES

    "src/render/renderer.cpp" 
    "src/render/renderer.h"
    "src/core/application.cpp" 
    "src/core/application.h"
  "src/material/texture.h")

 set(RENDER_SOURCES
    "src/model/model.h"
    "src/material/shader.h"
    "src/material/shader.cpp"
  "src/model/model.cpp" 
  "src/material/material.h" 
  "src/model/mesh.h" 
  "src/model/mesh.cpp" 
  "src/material/material.cpp" "src/material/texture.h" "src/material/texture.cpp")

set(UTIL_SOURCES
    "src/util/window.cpp" 
    "src/util/window.h" 
    "src/util/config.h"
  
 )

# Add executable
add_executable(OpenGL_Render 
    "src/main.cpp"
    ${CORE_SOURCES} 
    ${UTIL_SOURCES}
    ${RENDER_SOURCES}
 "vendor/stb_image/stb_iamge.cpp" "src/material/texture.h" "src/material/texture.cpp")

# Copy shader files
set(SHADER_SOURCE_DIR "${CMAKE_SOURCE_DIR}/shader")
set(SHADER_DEST_DIR "${CMAKE_BINARY_DIR}/shader")
file(MAKE_DIRECTORY ${SHADER_DEST_DIR})
add_custom_command(TARGET OpenGL_Render PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${SHADER_SOURCE_DIR} ${SHADER_DEST_DIR})

# Copy res files
set(RES_SOURCE_DIR "${CMAKE_SOURCE_DIR}/res")
set(RES_DEST_DIR "${CMAKE_BINARY_DIR}/res")
file(MAKE_DIRECTORY ${RES_DEST_DIR})
add_custom_command(TARGET OpenGL_Render PRE_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_directory
					   ${RES_SOURCE_DIR} ${RES_DEST_DIR})

# Link libraries
target_link_libraries(OpenGL_Render OpenGL::GL glfw libglew_static) 
