#ifndef DRAW_H
#define DRAW_H

#include <stdbool.h>
#include "model.h"
#include "basic_primitives.h"

typedef struct Rotate {
    double jupiter_rotation;
    double jupiter_moon_rotation;
    double venus_rotation;
    double saturnus_rotation;
    double satellite_rotation;
    double sun_rotation;
} Rotate;

typedef union Position {
    struct {
        double x;
        double y;
        double z;
    };
    double Position[3];
} Position;

typedef union Move {
    struct {
        Position jupiter;
        Position jupiter_moon;
        Position venus;
        Position saturnus;
        Position sun;
        Position satellite;
    };
    Position Move[6];
} Move;

extern Rotate rotate;
extern Move move;

//Draw the model.
void draw_model(const struct Model *model);

//Draw the quads of the model.
void draw_quads(const struct Model *model);

//Draw the bottom skybox.
void draw_skybox(Entity skybox, int z_sign);

//Draw the entities to the world.
void draw_environment(World *world, Rotate *rotate, Move *move, double timer);

bool is_point_inside_spheres(double x, double y, double z, double x2, double y2, double z2, double radius);

void reshape(GLsizei width, GLsizei height);

void draw_help();

void display();

void set_satellite_led_working_time();

void idle();

#endif
