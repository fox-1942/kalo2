#ifndef CAMERA_H
#define CAMERA_H

#include "model.h"

typedef struct Camera {
    struct Vertex position;
    struct Vertex pose;
    struct Vertex prev_position;
} Camera;

typedef struct Action {
    int move_forward;
    int move_backward;
    int step_left;
    int step_right;
    int move_up;
    int move_down;
    int rotate_planets_in_galaxy;
    int move_jupiter_plus_moon_in_galaxy;
    int move_venus_in_galaxy;
    int move_saturnus_in_galaxy;
    int call_satellite;
    int increase_light;
    int decrease_light;
} Action;

extern Camera camera;
extern Action action;


//Checks if the camera can move. If it can't, loads the starter position.
void don_not_head_up_against_the_wall(Camera *camera, Move *move);


//Initialize the camera position and direction.
void init_camera(struct Camera *camera);


//Transform the models into the view point of the camera.
void set_view_point(const struct Camera *camera);


//Rotate the camera horizontally and vertically.
void rotate_camera(struct Camera *camera, double horizontal, double vertical);


//Move the camera forward.
void move_camera_forward(struct Camera *camera, double distance);


//Move the camera backward.
void move_camera_backward(struct Camera *camera, double distance);


//Step the camera left.
void step_camera_left(struct Camera *camera, double distance);


//Step the camera right.
void step_camera_right(struct Camera *camera, double distance);


//Move the camera up.
void move_camera_up(struct Camera *camera, double distance);

//Move the camera down.
void move_camera_down(struct Camera *camera, double distance);

void update_camera_position(Camera *camera, Action *action, Move *move, GLfloat *light_ambient, double elapsed_time);

#endif
