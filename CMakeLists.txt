cmake_minimum_required(VERSION 3.26)
project(CGAME C)

set(CMAKE_C_STANDARD 99)

option(BUILD_EMSCRIPTEN "Build game for web browsers" OFF)

option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)

add_subdirectory(vendor/glfw)

if (BUILD_EMSCRIPTEN)
    # set .html here so that we get .html output file.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "index")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -o index.html -s USE_WEBGL2=1 -s USE_GLFW=3 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_RUNTIME_METHODS=cwrap,allocate --shell-file ../shell_fullscreen.html --preload-file data")
endif ()

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

FILE(COPY shaders DESTINATION ${PROJECT_BINARY_DIR})
