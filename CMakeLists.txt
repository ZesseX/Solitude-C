cmake_minimum_required(VERSION 3.27)
project(Solitude C)

set(CMAKE_C_STANDARD 99)

set(GLFW_INCLUDE_DIR "C:/libs/glfw-3.3.9.bin.WIN64/include")
set(GLFW_LIB_DIR "C:/libs/glfw-3.3.9.bin.WIN64/lib-mingw-w64")

set(GLAD_INCLUDE_DIR "C:/libs/glad/include")
set(GLAD_SOURCE_DIR "C:/libs/glad/src")
include_directories(${GLAD_INCLUDE_DIR})
add_library(glad ${GLAD_SOURCE_DIR}/glad.c)

include_directories(${GLFW_INCLUDE_DIR})
link_directories(${GLFW_LIB_DIR})

set(PTHREADS_INCLUDE_DIR "C:/libs/pthreads/Pre-built.2/include")
set(PTHREADS_LIB_DIR "C:/libs/pthreads/Pre-built.2/dll/x86")

add_subdirectory(src)
add_subdirectory(test)