#ifndef DRAW_H
#define DRAW_H

#include "model.h"

#define SKYBOX_SIZE 10000.0

typedef struct Position {
    double x;
    double y;
    double z;
} Position;

typedef struct Rotate {
    double planet1_rotation;
    double planet2_rotation;
    double planet3_rotation;
    double planet4_rotation;
    double satellite_rotation;
    double sun_rotation;
} Rotate;

typedef struct Move {
    Position planet1;
    Position planet2;
    Position planet3;
    Position planet4;
    Position sun;
    Position satellite;
} Move;

//Draw the model.
void draw_model(const struct Model *model);

//Draw the triangles of the model.
void draw_triangles(const struct Model *model);

//Draw the quads of the model.
void draw_quads(const struct Model *model);

//Draw the normal vectors of the model.
void draw_normals(const struct Model *model, double length);


//Draw the top skybox.
void draw_skybox_top(Entity skybox);


//Draw the bottom skybox.
void draw_skybox_bottom(Entity skybox);


//Draw the entitys to the world.
void draw_environment(World world, Rotate *rotate, Move move);

bool is_point_inside_sphere(Move mov, Move mov2, Model planet);

#endif
