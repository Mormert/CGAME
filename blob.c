
#include "blob.h"
#include "game.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#else

#include <glad/glad.h>

#endif

#include <GLFW/glfw3.h>
#include "render.h"

#include "math.h"
#include "assert.h"

BlobCompare find_blob_state(float blobMass, float otherBlobMass) {
    if (blobMass < otherBlobMass * 0.9f) {
        return SmallerThanOther;
    } else if (blobMass > otherBlobMass * 1.1f) {
        return LargerThanOther;
    } else {
        return MergeableWithOther;
    }
}

static void blob_collisions_resolve(Blob *blob) {
    float mass = blob_get_mass(blob);
    float blobRadius = blob_get_radius(blob);

    for (int i = 0; i < MAX_BLOBS; i++) {
        Blob *otherBlob = &gGame.blobs[i];
        if (!otherBlob->alive || otherBlob == blob) {
            continue;
        }

        vec2 diff;
        vec2_sub(diff, blob->pos, otherBlob->pos);

        float distance = vec2_len(diff);
        float massOther = blob_get_mass(otherBlob);

        if (distance < blobRadius) {
            BlobCompare compare = find_blob_state(mass, massOther);
            if (compare == MergeableWithOther) {
                mass += massOther;
                blob_set_mass(blob, mass);
                otherBlob->alive = 0;
            } else if (compare == LargerThanOther) {
                mass -= massOther;
                blob_set_mass(blob, mass);
                otherBlob->alive = 0;
            } else { // Merge
                massOther += mass;
                blob_set_mass(otherBlob, massOther);
                blob->alive = 0;
            }

            game_spawn_blob();
            return;
        }
    }
}

static void blob_apply_movement(Blob *blob, double dt) {
    vec2_add(blob->pos, blob->pos, blob->velocity);

    const float friction = 5.0f;

    float speed = vec2_len(blob->velocity);

    float maxSpeed = 1.f;
    if (blob->isPlayer) {
        maxSpeed = 5.f;
    }

    if (speed > maxSpeed) {
        vec2_norm(blob->velocity, blob->velocity);
        vec2_scale(blob->velocity, blob->velocity, maxSpeed);
    } else {
        float newSpeed = max(0, speed - friction * dt);

        if (speed > 0.f) {
            vec2_norm(blob->velocity, blob->velocity);
            vec2_scale(blob->velocity, blob->velocity, newSpeed);
        }
    }
}

void blobs_update(double dt) {
    for (int i = 0; i < MAX_BLOBS; i++) {
        Blob *blob = &gGame.blobs[i];
        if (!blob->alive) {
            continue;
        }
        blob_apply_movement(blob, dt);
        blob_collisions_resolve(blob);
    }
}

void blobs_render() {
    shader_use(gGame.circleShaderPlayer);
    shader_set_float(gGame.circleShaderPlayer, "time", glfwGetTime());

    for (int i = 0; i < MAX_BLOBS; i++) {
        const Blob *blob = &gGame.blobs[i];
        if (!blob->alive) {
            return;
        }

        mat4x4 M;
        mat4x4_identity(M);
        mat4x4_translate_in_place(M, blob->pos[0], blob->pos[1], 0.f);

        float radius = blob_get_radius(blob);
        mat4x4_scale_aniso(M, M, radius, radius, 1.f);

        vec3 color;

        if (blob == &gGame.blobs[gGame.playerBlobIndex]) {
            color[0] = 0.9f;
            color[1] = 0.8f;
            color[2] = 0.9f;
        } else {
            BlobCompare compare = find_blob_state(blob_get_mass(blob),
                                                  blob_get_mass(&gGame.blobs[gGame.playerBlobIndex]));
            switch (compare) {
                case LargerThanOther:
                    color[0] = 0.8f;
                    color[1] = 0.2f;
                    color[2] = 0.1f;
                    break;
                case SmallerThanOther:
                    color[0] = 0.8f;
                    color[1] = 0.5f;
                    color[2] = 0.1f;
                    break;
                case MergeableWithOther:
                    color[0] = 0.1f;
                    color[1] = 0.8f;
                    color[2] = 0.2f;
                    break;
                default:
                    break;
            }
        }

        shader_set_vec3(gGame.circleShaderPlayer, "color", color);
        render_quad(&M, gGame.circleShaderPlayer);
    }

}

void blob_set_mass(Blob *blob, float mass) {
    if (mass < 100.f || isnan(mass)) {
        mass = 100.f;
    }

    blob->pMass = mass;
    blob->pRadius = sqrtf(mass / 3.14159265f);
}

float blob_get_mass(Blob *blob) {
    return blob->pMass;
}

float blob_get_radius(Blob *blob) {
    return blob->pRadius;
}

void blob_add_velocity(Blob *blob, vec2 velocity) {
    if (velocity[0] > 12.f) {
        velocity[0] = 12.f;
    }
    if (velocity[1] > 12.f) {
        velocity[1] = 12.f;
    }
    vec2_add(blob->velocity, blob->velocity, velocity);
}
