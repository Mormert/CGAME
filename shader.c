
#include "shader.h"

Shader *shader_create(const char *vertexPath, const char *fragmentPath) {
    FILE* vertexFile = fopen(vertexPath, "rb");
    if (vertexFile == NULL) {
        fprintf(stderr, "Failed to open vertex shader file\n");
        return NULL;
    }
    fseek(vertexFile, 0, SEEK_END);
    long vertexSize = ftell(vertexFile);
    rewind(vertexFile);

    char* vertexShaderSource = malloc(vertexSize + 1);
    if (vertexShaderSource == NULL) {
        fprintf(stderr, "Failed to allocate memory for vertex shader source\n");
        fclose(vertexFile);
        return NULL;
    }
    fread(vertexShaderSource, 1, vertexSize, vertexFile);
    vertexShaderSource[vertexSize] = '\0';
    fclose(vertexFile);

    FILE* fragmentFile = fopen(fragmentPath, "rb");
    if (fragmentFile == NULL) {
        fprintf(stderr, "Failed to open fragment shader file\n");
        free(vertexShaderSource);
        return NULL;
    }
    fseek(fragmentFile, 0, SEEK_END);
    long fragmentSize = ftell(fragmentFile);
    rewind(fragmentFile);

    char* fragmentShaderSource = malloc(fragmentSize + 1);
    if (fragmentShaderSource == NULL) {
        fprintf(stderr, "Failed to allocate memory for fragment shader source\n");
        fclose(fragmentFile);
        free(vertexShaderSource);
        return NULL;
    }
    fread(fragmentShaderSource, 1, fragmentSize, fragmentFile);
    fragmentShaderSource[fragmentSize] = '\0';
    fclose(fragmentFile);

    GLint success;
    char infoLog[512];

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**)&vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "Vertex Shader Compilation Failed\n%s\n", infoLog);
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "Fragment Shader Compilation Failed\n%s\n", infoLog);
    }

    Shader* shader = malloc(sizeof(Shader));
    shader->id = glCreateProgram();
    glAttachShader(shader->id, vertexShader);
    glAttachShader(shader->id, fragmentShader);
    glLinkProgram(shader->id);

    glBindFragDataLocation(shader->id, 0, "FragColor");

    glGetProgramiv(shader->id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader->id, 512, NULL, infoLog);
        fprintf(stderr, "Shader Program Linking Failed\n%s\n", infoLog);
    }

    free(vertexShaderSource);
    free(fragmentShaderSource);

    return shader;
}

void shader_destroy(Shader *shader) {
    glDeleteProgram(shader->id);
    free(shader);
}

void shader_set_mat4(Shader *shader, char *location, mat4x4 *mat) {
    GLint ul = glGetUniformLocation(shader->id, location);
    glUniformMatrix4fv(ul, 1, GL_FALSE, (const GLfloat*) mat);
}

void shader_set_float(Shader *shader, char *location, float value) {
    GLint ul = glGetUniformLocation(shader->id, location);
    glUniform1f(ul, value);
}

void shader_set_vec3(Shader *shader, char *location, vec3 value) {
    GLint ul = glGetUniformLocation(shader->id, location);
    glUniform3f(ul, value[0], value[1], value[2]);
}

void shader_use(Shader *shader) {
    glUseProgram(shader->id);
}


