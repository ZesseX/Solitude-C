file(GLOB_RECURSE SOURCE_FILES "*.c")
file(GLOB_RECURSE HEADER_FILES "*.h")
#file(GLOB_RECURSE ASM_FILES "*.s")

add_library(SolitudeLib ${SOURCE_FILES} ${HEADER_FILES})

add_executable(Solitude ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(Solitude opengl32 gdi32 glfw3 glad)
target_link_libraries(SolitudeLib opengl32 gdi32 glfw3 glad)
