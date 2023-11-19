
#ifndef CGAME_BLOB_H
#define CGAME_BLOB_H

#include <linmath.h>

typedef enum {
    SmallerThanOther,
    MergeableWithOther,
    LargerThanOther
} BlobCompare;

BlobCompare find_blob_state(float blobMass, float otherBlobMass);

typedef struct {
    char alive;
    char isPlayer;

    vec2 pos, velocity;

    // Set using blob_set_mass()
    float pMass;

    // Access using blob_get_radius()
    float pRadius;
} Blob;

void blob_set_mass(Blob *blob, float mass);

float blob_get_mass(Blob *blob);

float blob_get_radius(Blob *blob);

void blob_add_velocity(Blob *blob, vec2 velocity);

void blobs_update(double dt);

void blobs_render();

#endif //CGAME_BLOB_H
