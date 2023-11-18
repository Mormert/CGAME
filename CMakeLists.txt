cmake_minimum_required(VERSION 3.26)
project(CGAME C)

set(CMAKE_C_STANDARD 99)

option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)

add_subdirectory(vendor/glfw)

add_executable(CGAME engine.c
        shader.c
        render.c
        vendor/glad/src/glad.c
        game.c
        game.h
        player.c
        player.h
        engine.h
        enemy.c
        enemy.h)

find_package(OpenGL REQUIRED)

target_include_directories(CGAME PUBLIC
        ${OPENGL_INCLUDE_DIR}
        vendor/glfw/include
        vendor/glad/include
        vendor)

target_link_libraries(CGAME glfw
        ${OPENGL_gl_LIBRARY})
