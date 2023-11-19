
#include "game.h"

#include "render.h"
#include "blob.h"

#include <assert.h>

Game gGame;

static float randomNearby(float value, float margin) {
    float offset = margin * value;

    float randOffset = (float) rand() / RAND_MAX * 2 * offset - offset;
    float nearbyValue = value + randOffset;

    return nearbyValue;
}

int game_spawn_blob() {
    for (int i = 0; i < MAX_BLOBS; i++) {
        Blob *blob = &gGame.blobs[i];
        if (!blob->alive) {
            blob->alive = 1;
            float newMass = randomNearby(blob_get_mass(&gGame.blobs[gGame.playerBlobIndex]), 0.4f);
            if (newMass < 1000.f) {
                newMass = 1000.f;
            }
            blob_set_mass(blob, newMass);
            blob->pos[0] = rand() % 2501 - 1250;
            blob->pos[1] = rand() % 2501 - 1250;
            blob->velocity[0] = (rand() % 3 - 2.f) * 0.2f;
            blob->velocity[0] = (rand() % 3 - 2.f) * 0.2f;
            return i;
        }
    }
    assert(0);
    return 0;
}

void game_init() {
    gGame.quadShader = shader_create("shaders/quad.vert", "../shaders/quad.frag");
    gGame.circleShader = shader_create("shaders/quad.vert", "../shaders/circle.frag");
    gGame.circleShaderPlayer = shader_create("shaders/quad.vert", "../shaders/circle_player.frag");

    gGame.grid = grid_create();
    gGame.playerBlobIndex = game_spawn_blob();
    gGame.blobs[gGame.playerBlobIndex].isPlayer = 1;

    for (int i = 0; i < INITIAL_BLOBS - 1; i++) {
        game_spawn_blob();
    }
}

void game_destroy() {
    shader_destroy(gGame.quadShader);
    shader_destroy(gGame.circleShader);
    shader_destroy(gGame.circleShaderPlayer);
    grid_destroy(gGame.grid);
}

void game_render() {
    grid_render(gGame.grid);
    blobs_render();
}

static void game_ai() {
    for (int i = 1; i < MAX_BLOBS; i++) {
        Blob *aiBlob = &gGame.blobs[i];

        vec2 v = {0.f, 0.0f};

        for (int j = 0; j < MAX_BLOBS; j++) {
            Blob *otherBlob = &gGame.blobs[j];
            if (otherBlob == aiBlob || !otherBlob->alive) {
                continue;
            }
            vec2 diff;
            vec2_sub(diff, otherBlob->pos, aiBlob->pos);
            float len = vec2_len(diff);
            if (len > 450.f) {
                continue;
            }
            float relevance = 1 / (len * len);
            vec2_scale(diff, diff, relevance * 15.0f);
            BlobCompare compare = find_blob_state(blob_get_mass(aiBlob), blob_get_mass(otherBlob));
            if (otherBlob == &gGame.blobs[gGame.playerBlobIndex]) {
                if (compare == MergeableWithOther) {
                    vec2_sub(v, v, diff);
                }
            } else {
                if (compare == MergeableWithOther) {
                    vec2_add(v, v, diff);
                } else {
                    vec2_sub(v, v, diff);
                }
            }
        }

        blob_add_velocity(aiBlob, v);
    }
}

void game_loop(double dt) {
    game_render();

    player_blob_update(&gGame.blobs[gGame.playerBlobIndex], dt);
    game_ai();
    blobs_update(dt);
}

/*
float game_get_mass_level() {
    float mass = 3.14159265f * gGame.player.radius * gGame.player.radius;
    if (isnan(mass) || mass < 25.f) {
        mass = 25.f;
    }
    const float defaultMass = 3.14159265f * 25.f * 25.f;
    return mass / defaultMass;

    return 1.f;
}

*/


