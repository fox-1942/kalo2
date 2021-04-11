#ifndef DRAW_H
#define DRAW_H

#include "controller.h"

#define SKYBOX_SIZE 10000.0

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
void draw_skybox(Entity skybox, int z_sign);

//Draw the entities to the world.
void draw_environment(World *world, Rotate *rotate, Move *move, double e_time);

bool is_point_inside_spheres(double x, double y, double z, double x2, double y2, double z2, double radius);

double calc_elapsed_for_led();

void reshape(GLsizei width, GLsizei height);

void draw_help();

void display();

void set_satellite_led_working_time();

void idle();


#endif
