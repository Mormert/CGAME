
#include "grid.h"

#include "render.h"

#define GRID_SIZE 50

void grid_setup(Grid* grid){

    const int gridSize = GRID_SIZE;
    const int bytes = gridSize * 4 * 2 + 20000; // this is a bug, we add 20000 here to avoid a crash lol
    float* segments = malloc(bytes); // each line requires 4 float values, grid is two-dimensional (horizontal + vertical)

    int idx = 0;
    float gapSize = 2.0f / gridSize;
    for(float i = -1.0f; i < 1.0f; i += gapSize) {
        segments[idx++] = i; segments[idx++] = -1.0f;  // horizontal lines
        segments[idx++] = i; segments[idx++] = 1.0f;
        segments[idx++] = -1.0f; segments[idx++] = i;  // vertical lines
        segments[idx++] = 1.0f; segments[idx++] = i;
    }

    glGenVertexArrays(1, &grid->VAO);
    glGenBuffers(1, &grid->VBO);
    glBindVertexArray(grid->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, grid->VBO);
    glBufferData(GL_ARRAY_BUFFER, bytes, segments, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    free(segments);

}

Grid* grid_create() {
    Grid* grid = malloc(sizeof(Grid));
    grid->shader = shader_create("shaders/line.vert", "shaders/line.frag");

    grid_setup(grid);

    return grid;
}

void grid_destroy(Grid *grid) {
    shader_destroy(grid->shader);

    glDeleteBuffers(1, &grid->VBO);
    glDeleteVertexArrays(1, &grid->VAO);

    free(grid);
}

void grid_render(Grid* grid) {
    shader_use(grid->shader);

    mat4x4 M;
    mat4x4_identity(M);
    mat4x4_scale_aniso(M, M, 3000.f, 3000.f, 3000.f);
    shader_set_mat4(grid->shader, "model", &M);

    render_set_view_proj_from_state(grid->shader);

    glBindVertexArray(grid->VAO);
    glDrawArrays(GL_LINES, 0, 2 * GRID_SIZE * 2 * 2);
}
