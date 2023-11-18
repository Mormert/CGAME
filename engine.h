
#ifndef CGAME_ENGINE_H
#define CGAME_ENGINE_H

struct GLFWwindow;

typedef struct  {
    struct GLFWwindow* window;
    float cameraZoom;
} Engine;

extern Engine gEngine;

int main();

#endif //CGAME_ENGINE_H
