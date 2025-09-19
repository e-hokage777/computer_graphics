cmake_minimum_required(VERSION 3.10)

project(model_loading VERSION 1.0.0  LANGUAGES CXX C)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/main.cpp ${PROJECT_SOURCE_DIR}/src/glad.c)

target_link_directories(${PROJECT_NAME} PUBLIC libs)

target_link_libraries(${PROJECT_NAME} glfw3)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)