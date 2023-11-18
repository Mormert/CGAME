#include "engine.h"

#include <stdio.h>
#include <math.h>

#include "render.h"
#include "game.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <time.h>

Engine gEngine;

void update_window_and_projection(GLFWwindow* window){
    mat4x4 proj;
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    width *= gEngine.cameraZoom;
    height *= gEngine.cameraZoom;
    mat4x4_ortho(proj, -width / 2.f, width / 2.f, - height/2.f, height/2.f, 2.f, -2.f);
    render_set_projection(&proj);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    gEngine.cameraZoom -= yoffset * 0.05f;
    if(gEngine.cameraZoom < 0.5f){
        gEngine.cameraZoom = 0.5f;
    }
}

void engine_init(){
    gEngine.cameraZoom = 1.f;
}

void loop(){
    double frameTime = glfwGetTime();
    while (!glfwWindowShouldClose(gEngine.window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(sinf(glfwGetTime()) * 0.5f, 0.2f, 0.5f, 1.0f);

        update_window_and_projection(gEngine.window);

        double thisFrameTime = glfwGetTime();
        game_loop(thisFrameTime - frameTime);
        frameTime = thisFrameTime;

        glfwSwapBuffers(gEngine.window);

        glfwPollEvents();
    }

}


int main() {

    srand(time(0));

    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    gEngine.window = glfwCreateWindow(720, 720, "C GAME", NULL, NULL);
    if (!gEngine.window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(gEngine.window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD.\n");
        return -1;
    }

    glfwSetScrollCallback(gEngine.window, scroll_callback);

    render_init();
    engine_init();
    game_init();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, true);
#else
    loop();
#endif

    game_destroy();
    render_destroy();

    glfwTerminate();
    return 0;
}