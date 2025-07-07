cmake_minimum_required(VERSION 3.10)

project(lighting VERSION 1.0.0 LANGUAGES C CXX)

set(EXEC_SOURCES "src/main.cpp" "src/gl.c" "src/stb_image.cpp")

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glfw-3.4/glfw-3.4 glfw)

find_package(OpenGL)

link_libraries(glfw OpenGL::GL)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${EXEC_SOURCES})