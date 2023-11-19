
#include "player.h"
#include "render.h"

#include "game.h"
#include "blob.h"
#include "engine.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#else

#include <glad/glad.h>

#endif

#include <GLFW/glfw3.h>

#define GLT_IMPLEMENTATION

#include "gltext.h"

static void player_get_mat(Blob *playerBlob, mat4x4 M) {
    mat4x4_identity(M);
    mat4x4_translate_in_place(M, playerBlob->pos[0], playerBlob->pos[1], 0.f);
    float radius = blob_get_radius(playerBlob);
    mat4x4_scale_aniso(M, M, radius, radius, 1.f);
}

static void player_set_camera_view(Blob *playerBlob) {
    mat4x4 view;
    mat4x4_identity(view);
    mat4x4_translate_in_place(view, playerBlob->pos[0], playerBlob->pos[1], 0.f);
    mat4x4_invert(view, view);
    render_set_view(&view);
}

void player_blob_update(Blob *playerBlob, double dt) {

    vec2 addVelocity = {0.f, 0.f};

    if (glfwGetKey(gEngine.window, 'A')) {
        addVelocity[0] -= dt * 12.f;
    }

    if (glfwGetKey(gEngine.window, 'D')) {
        addVelocity[0] += dt * 12.f;
    }

    if (glfwGetKey(gEngine.window, 'W')) {
        addVelocity[1] += dt * 12.f;
    }

    if (glfwGetKey(gEngine.window, 'S')) {
        addVelocity[1] -= dt * 12.f;
    }

    blob_add_velocity(playerBlob, addVelocity);

    player_set_camera_view(playerBlob);
}

