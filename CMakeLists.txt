cmake_minimum_required(VERSION 3.27)
project(Solitude C)

set(CMAKE_C_STANDARD 99)

set(GLFW_INCLUDE_DIR "C:/libs/glfw-3.3.9.bin.WIN64/include")
set(GLFW_LIB_DIR "C:/libs/glfw-3.3.9.bin.WIN64/lib-mingw-w64")

include_directories(${GLFW_INCLUDE_DIR})
link_directories(${GLFW_LIB_DIR})

add_subdirectory(src)
add_subdirectory(test)