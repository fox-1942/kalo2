#include "model.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <load.h>

void init_model(Model* model)
{
    model->n_vertices = 1;
    model->n_texture_vertices = 1;
    model->n_normals = 1;
    model->n_triangles = 0;
    model->n_quads = 0;

}

void allocate_model(Model* model)
{
    model->vertices = (struct Vertex *) malloc((model->n_vertices + 1) * sizeof(struct Vertex));
    model->texture_vertices = (struct TextureVertex *) malloc(
            (model->n_texture_vertices + 1) * sizeof(struct TextureVertex));
    model->normals = (struct Vertex *) malloc((model->n_normals + 1) * sizeof(struct Vertex));
    model->triangles = (struct Triangle *) malloc(model->n_triangles * sizeof(struct Triangle));
    model->quads = (struct Quad *) malloc(model->n_quads * sizeof(struct Quad));
}

void free_model(Model* model)
{
    if (model->vertices != NULL) {
        free(model->vertices);
    }
    if (model->texture_vertices != NULL) {
        free(model->texture_vertices);
    }
    if (model->normals != NULL) {
        free(model->normals);
    }
    if (model->triangles != NULL) {
        free(model->triangles);
    }
}

void calc_bounding_box(struct Model *model) {
    int i;
    double x, y, z;
    double min_x, max_x, min_y, max_y, min_z, max_z;

    if (model->n_vertices == 0) { return; }

    min_x = model->vertices[1].x;
    max_x = model->vertices[1].x;
    min_y = model->vertices[1].y;
    max_y = model->vertices[1].y;
    min_z = model->vertices[1].z;
    max_z = model->vertices[1].z;

    for (i = 1; i < model->n_vertices; ++i) {
        x = model->vertices[i].x;
        y = model->vertices[i].y;
        z = model->vertices[i].z;
        if (x < min_x) {
            min_x = x;
        } else if (x > max_x) {
            max_x = x;
        }
        if (y < min_y) {
            min_y = y;
        } else if (y > max_y) {
            max_y = y;
        }
        if (z < min_z) {
            min_z = z;
        } else if (z > max_z) {
            max_z = z;
        }
    }
    /*printf("Bounding box:\n");
    printf("x in [%lf, %lf]\n", min_x, max_x);
    printf("y in [%lf, %lf]\n", min_y, max_y);
    printf("z in [%lf, %lf]\n", min_z, max_z);*/

    // setting the coordinates of the bounding box for the specific model
    model->box.minVertex.x = min_x;
    model->box.minVertex.y = min_y;
    model->box.minVertex.z = min_z;
    model->box.maxVertex.x = max_x;
    model->box.maxVertex.y = max_y;
    model->box.maxVertex.z = max_z;

    model->box.diagonal_length = vector_length(min_x, min_y, min_z, max_x, max_y, max_z);
    printf("Bbox diagonal length: %f \n", model->box.diagonal_length);

}

void scale_model(struct Model *model, double sx, double sy, double sz) {
    int i;

    for (i = 0; i < model->n_vertices; ++i) {
        model->vertices[i].x *= sx;
        model->vertices[i].y *= sy;
        model->vertices[i].z *= sz;
    }
}

void init_entities(World *world) {
    //Load the planet1 object and texture.
    load_model("..\\objects\\geoid.obj", &world->jupiter.model);
    world->jupiter.texture = load_texture("..\\textures\\planet2.png");
    scale_model(&world->jupiter.model, 0.4, 0.4, 0.4);

    //Load the planet2 and texture.
    load_model("..\\objects\\geoid.obj", &world->jupiter_moon.model);
    world->jupiter_moon.texture = load_texture("..\\textures\\planet3.png");
    scale_model(&world->jupiter_moon.model, 0.2, 0.2, 0.2);

    //Load the planet3 and texture.
    load_model("..\\objects\\geoid.obj", &world->venus.model);
    world->venus.texture = load_texture("..\\textures\\planet1.png");
    scale_model(&world->venus.model, 0.5, 0.5, 0.5);

    //Load the planet4 and texture.
    load_model("..\\objects\\saturn.obj", &world->saturnus.model);
    world->saturnus.texture = load_texture("..\\textures\\planet4.jpg");
    scale_model(&world->saturnus.model, 0.2, 0.2, 0.2);

    //Load the sun object and texture.
    load_model("..\\objects\\geoid.obj", &world->sun.model);
    world->sun.texture = load_texture("..\\textures\\sun.png");
    scale_model(&world->sun.model, 2, 2, 2);

    //Load the satellite object and texture.
    load_model("..\\objects\\satellite.obj", &world->satellite.model);
    world->satellite.texture = load_texture("..\\textures\\satellite.jpg");
    world->satellite.texture2 = load_texture("..\\textures\\satellite2.jpg");

    scale_model(&world->satellite.model, 12, 12, 12);

    //Load the skybox texture.
    world->skybox.texture = load_texture("..\\textures\\sky.png");

    //Storing bounding-box coordinates in each model
    calc_bounding_box(&world->jupiter.model);
    calc_bounding_box(&world->jupiter_moon.model);
    calc_bounding_box(&world->venus.model);
    calc_bounding_box(&world->saturnus.model);
    calc_bounding_box(&world->sun.model);
    world->sun.model.box.diagonal_length-=10; // small optimalization for better demonstration.
}

double vector_length(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z) {
    double diagonal_x = max_x - min_x;
    double diagonal_y = max_y - min_y;
    double diagonal_z = max_z - min_z;

    return sqrt(pow(diagonal_x, 2) + pow(diagonal_y, 2) + pow(diagonal_z, 2));
}

Vertex vector_from_two_vertex(double a_x, double a_y, double a_z, double b_x, double b_y, double b_z) {
    return (Vertex) {.x =  a_x - b_x, .y = a_y - b_y, .z = a_z - b_z};
}
