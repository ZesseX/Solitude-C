file(GLOB_RECURSE TEST_SOURCE_FILES "*.c")
file(GLOB_RECURSE TEST_HEADER_FILES "*.h")

add_executable(SolitudeTest ${TEST_SOURCE_FILES})

target_link_libraries(SolitudeTest opengl32 gdi32 glfw3 glad)
