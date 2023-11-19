
#ifndef CGAME_RENDER_H
#define CGAME_RENDER_H

#include "shader.h"

#include <linmath.h>

struct RenderState{
    mat4x4 view;
    mat4x4 proj;
};

void render_set_view(mat4x4* view);
void render_set_projection(mat4x4* projection);

void render_quad(mat4x4* mat, Shader* shader);

void render_set_view_proj_from_state(Shader* shader);

void render_get_view(mat4x4 outView);
void render_get_proj(mat4x4 outProj);

void render_init();
void render_destroy();

#endif //CGAME_RENDER_H
