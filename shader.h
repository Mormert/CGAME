
#ifndef CGAME_SHADER_H
#define CGAME_SHADER_H

#include <OpenGL/gl3.h>
#include <stdio.h>
#include <stdlib.h>

#include <linmath.h>

typedef struct {
    GLuint id;
} Shader;

Shader* shader_create(const char* vertexPath, const char* fragmentPath);
void shader_destroy(Shader* shader);

void shader_use(Shader* shader);
void shader_set_mat4(Shader* shader, char* location, mat4x4* mat);
void shader_set_float(Shader* shader, char* location, float value);
void shader_set_vec3(Shader* shader, char* location, vec3 value);

#endif //CGAME_SHADER_H
