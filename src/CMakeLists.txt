cmake_minimum_required(VERSION 3.27)
project(Solitude C)

set(CMAKE_C_STANDARD 99)

set(GLFW_INCLUDE_DIR "C:/libs/glfw-3.3.9.bin.WIN64/include")
set(GLFW_LIB_DIR "C:/libs/glfw-3.3.9.bin.WIN64/lib-mingw-w64")

include_directories(${GLFW_INCLUDE_DIR})
link_directories(${GLFW_LIB_DIR})

file(GLOB_RECURSE SOURCE_FILES "*.c")
file(GLOB_RECURSE HEADER_FILES "*.h")

add_executable(Solitude ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(Solitude opengl32 gdi32 glfw3)