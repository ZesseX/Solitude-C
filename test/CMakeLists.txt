file(GLOB_RECURSE TEST_SOURCE_FILES "*.c")
file(GLOB_RECURSE TEST_HEADER_FILES "*.h")

include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE TEST_SOURCE_FILES "*.c")
file(GLOB_RECURSE TEST_HEADER_FILES "*.h")

add_executable(SolitudeTest ${TEST_SOURCE_FILES})

target_link_libraries(SolitudeTest SolitudeLib opengl32 gdi32 glfw3 glad)
