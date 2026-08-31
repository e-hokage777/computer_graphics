cmake_minimum_required(VERSION 3.10)

project(model_loading VERSION 1.0.0 LANGUAGES C CXX)


set(EXEC_SOURCES "src/model_loading.cpp" "src/stb_image.cpp")
set(DT_EXEC_SOURCES "src/depth-testing.cpp" "src/stb_image.cpp")
set(ST_EXEC_SOURCES "src/stencil_testing.cpp" "src/stb_image.cpp")
set(BT_EXEC_SOURCES "src/blending.cpp" "src/stb_image.cpp")
set(FC_EXEC_SOURCES "src/face-culling.cpp" "src/stb_image.cpp")
set(FB_EXEC_SOURCES "src/framebuffers.cpp" "src/stb_image.cpp")
set(CM_EXEC_SOURCES "src/cubemap.cpp" "src/stb_image.cpp")
set(RF_EXEC_SOURCES "src/reflect_refract.cpp" "src/stb_image.cpp")
set(UBO_EXEC_SOURCES "src/uniform_buffer_objects.cpp" "src/stb_image.cpp")
set(GEOM_EXEC_SOURCES "src/geom-shader.cpp" "src/stb_image.cpp")
set(EXP_EXEC_SOURCES "src/explosion.cpp" "src/stb_image.cpp")
set(VN_EXEC_SOURCES "src/view_normals.cpp" "src/stb_image.cpp")
set(IN_EXEC_SOURCES "src/instancing.cpp" "src/stb_image.cpp")
set(AST_EXEC_SOURCES "src/asteriods.cpp" "src/stb_image.cpp")
set(ASTI_EXEC_SOURCES "src/asteriods_instancing.cpp" "src/stb_image.cpp")
set(AA_EXEC_SOURCES "src/anti-aliasing.cpp" "src/stb_image.cpp")
set(AAO_EXEC_SOURCES "src/anti-aliasing-offscreen.cpp" "src/stb_image.cpp")
set(L_EXEC_SOURCES "src/lights.cpp" "src/stb_image.cpp")
set(LG_EXEC_SOURCES "src/lights-gamma-correction.cpp" "src/stb_image.cpp")
set(SHADOWS_EXEC_SOURCES "src/shadows.cpp" "src/stb_image.cpp")
set(TO_EXEC_SOURCES "src/test-orthographic.cpp" "src/stb_image.cpp")
set(POINT_SHADOWS_EXEC_SOURCES "src/point-shadows.cpp" "src/stb_image.cpp")


# ---------------------------------------------------------
# 1. Configure and Fetch GLFW
# ---------------------------------------------------------
include(FetchContent)
fetchcontent_declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

# ---------------------------------------------------------
# 2. Fetch GLM (Header-only math library)
# ---------------------------------------------------------
fetchcontent_declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)

# ---------------------------------------------------------
# 3. Fetch GLAD (OpenGL Loader)
# ---------------------------------------------------------
# Note: This fetches Glad v1. If using Glad v2, configuration varies.
fetchcontent_declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
    GIT_SHALLOW ON
)

# ---------------------------------------------------------
# 3. Fetch GLAD (assimp model loader)
# ---------------------------------------------------------
fetchcontent_declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v6.0.5
)

# ---------------------------------------------------------
# 3. Fetch stbi_image
# ---------------------------------------------------------
set(STB_INCLUDE_DIR ${CMAKE_BINARY_DIR}/_deps/stb)

file(MAKE_DIRECTORY ${STB_INCLUDE_DIR})

file(DOWNLOAD
    https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
    ${STB_INCLUDE_DIR}/stb_image.h
)


FetchContent_MakeAvailable(glfw glm glad assimp)

# ---------------------------------------------------------
# 5. Configure GLAD v2 Target Loader
# ---------------------------------------------------------
# Define your own custom target name (e.g., glad_gl_core_33) and request your version/profile.
# Include GLAD's CMake functions
add_subdirectory(${glad_SOURCE_DIR}/cmake glad_cmake)

glad_add_library(glad
    STATIC
    REPRODUCIBLE
    LOADER
    API gl:core=3.3
)

find_package(OpenGL REQUIRED)


add_executable(${PROJECT_NAME} ${EXEC_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})


add_executable(depth_testing ${DT_EXEC_SOURCES})
target_link_libraries(depth_testing PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(depth_testing PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(stencil_testing ${ST_EXEC_SOURCES})
target_link_libraries(stencil_testing PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(stencil_testing PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(blending ${BT_EXEC_SOURCES})
target_link_libraries(blending PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(blending PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(face_culling ${FC_EXEC_SOURCES})
target_link_libraries(face_culling PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(face_culling PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(framebuffers ${FB_EXEC_SOURCES})
target_link_libraries(framebuffers PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(framebuffers PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(cubemaps ${CM_EXEC_SOURCES})
target_link_libraries(cubemaps PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(cubemaps PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(reflect_refract ${RF_EXEC_SOURCES})
target_link_libraries(reflect_refract PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(reflect_refract PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(uniform_buffer_objects ${UBO_EXEC_SOURCES})
target_link_libraries(uniform_buffer_objects PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(uniform_buffer_objects PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(geom_shaders ${GEOM_EXEC_SOURCES})
target_link_libraries(geom_shaders PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(geom_shaders PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(explosion ${EXP_EXEC_SOURCES})
target_link_libraries(explosion PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(explosion PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(view_normals ${VN_EXEC_SOURCES})
target_link_libraries(view_normals PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(view_normals PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(instancing ${IN_EXEC_SOURCES})
target_link_libraries(instancing PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(instancing PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(asteriods ${AST_EXEC_SOURCES})
target_link_libraries(asteriods PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(asteriods PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(anti-aliasing ${AA_EXEC_SOURCES})
target_link_libraries(anti-aliasing PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(anti-aliasing PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(asteriods_instancing ${ASTI_EXEC_SOURCES})
target_link_libraries(asteriods_instancing PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(asteriods_instancing PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(anti-aliasing-offscreen ${AAO_EXEC_SOURCES})
target_link_libraries(anti-aliasing-offscreen PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(anti-aliasing-offscreen PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(lights ${L_EXEC_SOURCES})
target_link_libraries(lights PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(lights PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(lights-gamma-correction ${LG_EXEC_SOURCES})
target_link_libraries(lights-gamma-correction PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(lights-gamma-correction PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(shadows ${SHADOWS_EXEC_SOURCES})
target_link_libraries(shadows PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(shadows PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(test-orthographic ${TO_EXEC_SOURCES})
target_link_libraries(test-orthographic PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(test-orthographic PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})

add_executable(point-shadows ${POINT_SHADOWS_EXEC_SOURCES})
target_link_libraries(point-shadows PRIVATE glfw glad glm::glm OpenGL::GL assimp)
target_include_directories(point-shadows PRIVATE ${PROJECT_SOURCE_DIR}/include ${STB_INCLUDE_DIR})


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/shaders
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)
