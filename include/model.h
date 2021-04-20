#ifndef OBJ_MODEL_H
#define OBJ_MODEL_H

#include "basic_primitives.h"

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
    int size;
} Entity;

typedef struct World {
    Entity jupiter;
    Entity jupiter_moon;
    Entity venus;
    Entity saturnus;
    Entity sun;
    Entity satellite;
    Entity skybox;
    Entity *planets[7];
} World;

World world;

double vector_length(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z);

Vertex vector_from_two_vertex(double a_x, double a_y, double a_z, double b_x, double b_y, double b_z);

void init_entities(World *world);

void scale_model(struct Model *model, double sx, double sy, double sz);

void calc_bounding_box(struct Model *model);

#endif /* OBJ_MODEL_H */
