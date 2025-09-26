
cmake_minimum_required(VERSION 3.10.0)
project(Cmake VERSION 0.1.0 LANGUAGES C CXX)

# GLFW paths
set(GLFW_INCLUDE_DIR /Users/masih/Downloads/glfw-3.4.bin.MACOS/include)
set(GLFW_LIB_DIR /Users/masih/Downloads/glfw-3.4.bin.MACOS/lib-universal)

# GLAD
add_library(glad_obj OBJECT external/glad/glad.c)
set_target_properties(glad_obj PROPERTIES INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/external/glad")

add_executable(Cmake main.cpp $<TARGET_OBJECTS:glad_obj>)
target_include_directories(Cmake PUBLIC ${GLFW_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/external/glad)
target_link_directories(Cmake PRIVATE ${GLFW_LIB_DIR})
target_link_libraries(Cmake PRIVATE glfw3 "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreFoundation")

include(CTest)
enable_testing()

