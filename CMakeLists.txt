cmake_minimum_required(VERSION 3.11)
project(CGAME C)

set(CMAKE_C_STANDARD 99)

option(BUILD_EMSCRIPTEN "Build game for web browsers" OFF)

add_executable(CGAME
        engine.c
        shader.c
        render.c
        game.c
        game.h
        player.c
        player.h
        engine.h
        blob.c
        grid.c
        grid.h)

find_package(OpenGL REQUIRED)

target_include_directories(CGAME PUBLIC
        ${OPENGL_INCLUDE_DIR}
        vendor)

target_link_libraries(CGAME
        ${OPENGL_gl_LIBRARY})

if (BUILD_EMSCRIPTEN)
    # set .html here so that we get .html output file.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "index")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -o index.html -s USE_WEBGL2=1 -s USE_GLFW=3 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_RUNTIME_METHODS=cwrap,allocate --shell-file ../shell_fullscreen.html --preload-file shaders")
else()
    option(GLFW_BUILD_EXAMPLES OFF)
    option(GLFW_BUILD_TESTS OFF)
    option(GLFW_BUILD_DOCS OFF)

    add_subdirectory(vendor/glfw)

    target_link_libraries(CGAME
            glfw)

    # Compile also glad.c when building w/o emscripten
    target_sources(CGAME PRIVATE vendor/glad/src/glad.c)

    target_include_directories(CGAME PUBLIC
            vendor/glfw/include
            vendor/glad/include)

endif()

FILE(COPY shaders DESTINATION ${PROJECT_BINARY_DIR})
