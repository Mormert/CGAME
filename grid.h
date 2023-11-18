
#ifndef CGAME_GRID_H
#define CGAME_GRID_H

#include "shader.h"

typedef struct{
    Shader* shader;
    unsigned int VAO, VBO;
} Grid;

Grid* grid_create();
void grid_destroy(Grid* grid);

void grid_render(Grid* grid);

#endif //CGAME_GRID_H
