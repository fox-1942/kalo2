#ifndef OBJ_MODEL_H
#define OBJ_MODEL_H

#include "basic_primitives.h"

#define TRUE 1
#define FALSE 0

#define INVALID_VERTEX_INDEX 0

/**
 * Three dimensional model with texture
 */


typedef struct Model {
    int n_vertices;
    int n_texture_vertices;
    int n_normals;
    int n_triangles;
    int n_quads;
    Vertex *vertices;
    struct TextureVertex *texture_vertices;
    struct Vertex *normals;
    struct Triangle *triangles;
    struct Quad *quads;
    struct BBox box;
} Model;

typedef struct Entity {
    Model model;
    int texture;
    int texture2;
    float material_ambient[4];
} Entity;

typedef union World {
    struct {
        Entity jupiter;
        Entity jupiter_moon;
        Entity venus;
        Entity saturnus;
        Entity sun;
        Entity satellite;
        Entity skybox;
    };
    Entity World[7];
} World;

extern World world;

/**
 * Types of the considered elements
 */
typedef enum {
    NONE,
    VERTEX,
    TEXTURE_VERTEX,
    NORMAL,
    FACE
} ElementType;

/**
 * Initialize the model structure.
 */
void init_model(Model *model);

/**
 * Allocate model.
 */
void allocate_model(Model *model);

/**
 * Release the allocated memory of the model.
 */
void free_model(Model *model);

double vector_length(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z);

Vertex vector_from_two_vertex(double a_x, double a_y, double a_z, double b_x, double b_y, double b_z);

void init_entities(World *world);

void scale_model(struct Model *model, double sx, double sy, double sz);

#endif /* OBJ_MODEL_H */
