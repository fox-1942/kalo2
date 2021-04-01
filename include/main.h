//
// Created by Fox-1942 on 01/04/2021.
//

#ifndef KALO_MAIN_H
#define KALO_MAIN_H

#include "camera.h"
#include "draw.h"
#include "model.h"
#include <math.h>

#define resolution 1   // 1: 1920*1080   0: 1366*768
#define fullscreen 1   // 1: fullscreen  0: windowed

GLfloat light_position[] = {0, 0, 0, 0};
GLfloat light_ambient[] = {0.5, 0.5, 0.5, 0};
GLfloat light_diffuse[] = {0.5, 0.5, 0, 0};
GLfloat light_specular[] = {1, 1, 1, 0};

World world;
Rotate rotate;
Move move;

struct Camera camera;
struct Action action;

struct Action {
    int move_forward;
    int move_backward;
    int step_left;
    int step_right;
    int move_up;
    int move_down;
    int rotate_planet1_in_galaxy;
    int rotate_planet2_in_galaxy;
    int rotate_planet3_in_galaxy;
    int rotate_planet4_in_galaxy;
    int move_planet1_in_galaxy;
    int move_planet3_in_galaxy;
    int move_planet4_in_galaxy;
    int call_satellite;
    int increase_light;
    int decrease_light;
};

int WINDOW_WIDTH = 0;
int WINDOW_HEIGHT = 0;
int mouse_x = 0;
int mouse_y = 0;
int previous_time = 0;
int help, help_on = 0;
float speed = 70;
float angle = 135;
double degree1 = 0;
double degree3 = 0;
double degree4 = 0;
double distance_a1 = 4000;
double distance_b1 = 4000;
double distance_a3 = 6000;
double distance_b3 = 6000;
double distance_a4 = 2000;
double distance_b4 = 2000;
int satellite = 0;


#endif //KALO_MAIN_H
