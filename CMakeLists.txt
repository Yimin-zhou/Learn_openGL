cmake_minimum_required(VERSION 3.10)
project(OpenGL_Render VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

# Add subdirectories
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glew)
# Add ImGui source files
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/vendor/imgui)
file(GLOB IMGUI_SOURCES ${IMGUI_DIR}/*.cpp ${IMGUI_DIR}/*.h)
list(APPEND IMGUI_SOURCES ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp)
# Add ImGuizmo source files
set(IMGUIZMO_DIR ${CMAKE_SOURCE_DIR}/vendor/ImGuizmo)
file(GLOB IMGUIZMO_SOURCES ${IMGUIZMO_DIR}/*.cpp ${IMGUIZMO_DIR}/*.h)

# Add ImGuiFileDialog source files
set(IMGUIFILEDIALOG_DIR ${CMAKE_SOURCE_DIR}/vendor/ImGuiFileDialog)
file(GLOB IMGUIFILEDIALOG_SOURCES ${IMGUIFILEDIALOG_DIR}/*.cpp ${IMGUIFILEDIALOG_DIR}/*.h)

# Include directories
include_directories(vendor/glfw/include)
include_directories(vendor/glew/include)
include_directories(vendor/glm)
include_directories(vendor/tinyobjloader)
include_directories(vendor/stb_image)
include_directories(vendor/imgui)
include_directories(vendor/ImGuizmo)
include_directories(src)
include_directories(${IMGUI_DIR})
include_directories(${IMGUI_DIR}/backends)
include_directories(${IMGUIZMO_DIR})

# OpenGL package
find_package(OpenGL 4.3 REQUIRED)

# Add ImGui as a library
add_library(ImGui STATIC ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)

# Add ImGuizmo as a library
add_library(ImGuizmo STATIC ${IMGUIZMO_SOURCES})
target_include_directories(ImGuizmo PUBLIC ${IMGUIZMO_DIR})

# Add ImGuiFileDialog as a library
add_library(ImGuiFileDialog STATIC ${IMGUIFILEDIALOG_SOURCES})
target_include_directories(ImGuiFileDialog PUBLIC ${IMGUIFILEDIALOG_DIR})

# Core and utility source files
set(CORE_SOURCES

    "src/render/forwardRenderer.cpp" 
    "src/render/forwardRenderer.h"
    "src/core/application.cpp" 
    "src/core/application.h"
 "src/render/deferredRenderer.cpp" "src/render/deferredRenderer.h" "src/render/renderer.h" "src/render/gbuffer.h")

 set(RENDER_SOURCES
    "src/model/model.h"
    "src/material/shader.h"
    "src/material/shader.cpp"
  "src/model/model.cpp" 
  "src/material/material.h" 
  "src/model/mesh.h" 
  "src/model/mesh.cpp" 
  "src/material/texture.h"
  "src/material/material.cpp" 
  "src/material/texture.h" 
  "src/material/texture.cpp"
    "src/camera/camera.h"
    "src/camera/camera.cpp"
    "src/render/light/light.h"
    "src/render/light/light.cpp"
    "src/render/light/directionalLight.h"
    "src/render/light/directionalLight.cpp"
    "src/render/light/pointLight.h"
    "src/render/light/pointLight.cpp"
    "src/render/light/spotLight.h"
    "src/render/light/spotLight.cpp"
    "src/render/light/lightManager.h"
    "src/render/light/lightManager.cpp"
  "src/effects/skyBox.h" "src/effects/skyBox.cpp")

set(UTIL_SOURCES
    "src/window/window.cpp" 
    "src/window/window.h" 
    "src/util/config.h"
  
  "src/render/scene.h" "src/render/scene.cpp" "src/window/imGuiManager.h" "src/window/imGuiManager.cpp")

# Add executable
add_executable(OpenGL_Render 
    "src/main.cpp"
    ${CORE_SOURCES} 
    ${UTIL_SOURCES}
    ${RENDER_SOURCES}
 "vendor/stb_image/stb_iamge.cpp" 
 "src/material/texture.h" 
 "src/material/texture.cpp" 
 "src/camera/camera.cpp")

# Copy shader files
set(SHADER_SOURCE_DIR "${CMAKE_SOURCE_DIR}/shader")
set(SHADER_DEST_DIR "${CMAKE_BINARY_DIR}/shader")
add_custom_target(CopyShaders ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Copying shaders..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADER_SOURCE_DIR} ${SHADER_DEST_DIR}
    COMMENT "Shaders copied to build directory."
)

# Copy res files
set(SHADER_SOURCE_DIR "${CMAKE_SOURCE_DIR}/res")
set(SHADER_DEST_DIR "${CMAKE_BINARY_DIR}/res")
add_custom_target(CopyRes ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Copying res..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADER_SOURCE_DIR} ${SHADER_DEST_DIR}
    COMMENT "res copied to build directory."
)

add_dependencies(OpenGL_Render CopyShaders)
add_dependencies(OpenGL_Render CopyRes)

# Link libraries
target_link_libraries(OpenGL_Render OpenGL::GL glfw libglew_static ImGui ImGuizmo ImGuiFileDialog) 
