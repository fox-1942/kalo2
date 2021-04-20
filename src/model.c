#include "model.h"
#include "load.h"
#include <math.h>

void scale_model(struct Model *model, double sx, double sy, double sz) {
    int i;

    for (i = 0; i < model->n_vertices; ++i) {
        model->vertices[i].x *= sx;
        model->vertices[i].y *= sy;
        model->vertices[i].z *= sz;
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

    model->box.minVertex.x = min_x;
    model->box.minVertex.y = min_y;
    model->box.minVertex.z = min_z;
    model->box.maxVertex.x = max_x;
    model->box.maxVertex.y = max_y;
    model->box.maxVertex.z = max_z;

    model->box.diagonal_length = vector_length(min_x, min_y, min_z, max_x, max_y, max_z);

}

void init_entities(World *world) {
    Entity *planetsToAdd[7] = {&world->jupiter, &world->jupiter_moon, &world->venus,
                               &world->saturnus, &world->sun, &world->satellite, &world->skybox};
    memcpy(world->planets, planetsToAdd, sizeof(planetsToAdd));

    for (int i = 0; i < 5; ++i) {
        if (i >= 0 && i <= 2 || i == 4) {
            load_model("..\\objects\\geoid.obj", &world->planets[i]->model);
            char msgOut[50];
            snprintf(msgOut,sizeof(msgOut), "..\\textures\\planet%d.png", i);
            world->planets[i]->texture = load_texture(msgOut);
        }
    }

    load_model("..\\objects\\saturn.obj", &world->saturnus.model);
    world->saturnus.texture = load_texture("..\\textures\\planet3.jpg");

    load_model("..\\objects\\satellite.obj", &world->satellite.model);
    world->satellite.texture = load_texture("..\\textures\\satellite.jpg");
    world->satellite.texture2 = load_texture("..\\textures\\satellite2.jpg");

    world->jupiter.size = 200;
    world->jupiter_moon.size = 85;
    world->venus.size = 220;
    world->saturnus.size = 150;
    world->sun.size = 920;
    world->satellite.size = 50;

    scale_model(&world->jupiter.model, 0.4, 0.4, 0.4);
    scale_model(&world->jupiter_moon.model, 0.2, 0.2, 0.2);
    scale_model(&world->venus.model, 0.5, 0.5, 0.5);
    scale_model(&world->saturnus.model, 0.2, 0.2, 0.2);
    scale_model(&world->sun.model, 2, 2, 2);
    scale_model(&world->satellite.model, 12, 12, 12);

    world->skybox.texture = load_texture("..\\textures\\sky.png");
    world->skybox.size = 6000;

    for (int i = 0; i < 6; i++) {
        calc_bounding_box(&world->planets[i]->model);
    }
    world->sun.model.box.diagonal_length -= 10; // small optimalization for better demonstration.

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
