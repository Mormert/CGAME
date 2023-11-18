
#include "render.h"
#include "shader.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#else
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

static GLuint quadVBO, quadVAO, quadEBO;
static struct RenderState renderState;

void rendering_quad_initialize() {

    float vertices[] = {
            -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,  // Top-left
            0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  // Top-right
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  // Bottom-right
            -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  // Bottom-left
    };

    unsigned int indices[] = {
            0, 1, 2,  // First Triangle
            2, 3, 0   // Second Triangle
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void rendering_quad_destroy() {
    glDeleteBuffers(1, &quadVBO);
    glDeleteBuffers(1, &quadEBO);
    glDeleteVertexArrays(1, &quadVAO);
}

void render_init() {
    rendering_quad_initialize();
}

void render_destroy() {
    rendering_quad_destroy();
}

void render_quad(mat4x4* mat, Shader* shader) {
    shader_use(shader);

    shader_set_mat4(shader, "model", mat);
    render_set_view_proj_from_state(shader);

    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void render_set_view(mat4x4 *view) {
    mat4x4_dup(renderState.view, *view);
}

void render_set_projection(mat4x4 *projection) {
    mat4x4_dup(renderState.proj, *projection);
}

void render_set_view_proj_from_state(Shader *shader) {
    shader_set_mat4(shader, "projection", &renderState.proj);
    shader_set_mat4(shader, "view", &renderState.view);
}
