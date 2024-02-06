file(GLOB_RECURSE SOURCE_FILES "*.c")
file(GLOB_RECURSE HEADER_FILES "*.h")

set(GLAD_INCLUDE_DIR "C:/libs/glad/include")
set(GLAD_SOURCE_DIR "C:/libs/glad/src")
include_directories(${GLAD_INCLUDE_DIR})
add_library(glad ${GLAD_SOURCE_DIR}/glad.c)

add_executable(Solitude ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(Solitude opengl32 gdi32 glfw3 glad)
