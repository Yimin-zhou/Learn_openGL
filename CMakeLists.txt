cmake_minimum_required(VERSION 3.10)
project(OpenGL_Render VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)


add_compile_definitions(GLEW_STATIC)

# add header files
include_directories(vendor/glfw/include)
include_directories(vendor/glew/include)

# add library files
find_package(OpenGL REQUIRED)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glew/build/cmake)

set(CORE_SOURCES
"src/core/window.cpp" 
"src/core/window.h" 
"src/core/renderer.cpp" 
"src/core/renderer.h"
"src/core/application.cpp" 
"src/core/application.h"
)

set(UTIL_SOURCES

)

add_executable(OpenGL_Render 
"src/main.cpp"
${CORE_SOURCES} 
${UTIL_SOURCES} )


target_link_libraries(OpenGL_Render OpenGL::GL glfw glew_s) 