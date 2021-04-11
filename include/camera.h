#ifndef CAMERA_H
#define CAMERA_H

#include "model.h"

#define skybox_size 6000
#define sun_size 920
#define planet1_size 200
#define planet2_size 85
#define planet3_size 220
#define planet4_size 150
#define satellite_size 50

#define MOVE_SPEED 10.0  //BIGGER IS FASTER
#define CAMERA_SPEED 5.0 //BIGGER IS SLOWER


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

void update_camera_position(Camera *camera, Action *action, Move *move, GLfloat *light_ambient, double elapsed_time,
                            double speed);

#endif
