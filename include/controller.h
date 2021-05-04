#ifndef KALO_CONTROLLER_H
#define KALO_CONTROLLER_H

#include "camera.h"
#include "draw.h"

#define RESOLUTION 0  // 1: 1920*1080   0: 1366*768
#define FULLSCREEN 0   // 1: fullscreen  0: windowed

typedef struct Window {
    int mouse_x;
    int mouse_y;
    int window_width;
    int window_height;
    double e_time;
    int previous_time;
} Window;

typedef struct Action {
    int rotate_planets_in_galaxy;
    int move_jupiter_plus_moon_in_galaxy;
    int move_venus_in_galaxy;
    int move_saturnus_in_galaxy;
    int call_satellite;
    int satellite_is_moving;
    int increase_light;
    int decrease_light;
    int help;
    int help_on;
    int fog;
    GLfloat light_ambient[4];
} Action;

Window window;
Action action;


void init_controller(Move *move);

void movement_of_objects(Move *move, Action *action, World *world);

void rotation_of_objects(Action *action, Rotate *rotate);

void mouse_handler(int button, int state, int x, int y);

void motion_handler(int x, int y);

void key_handler(unsigned char key, int x, int y);

void key_up_handler(unsigned char key, int x, int y);

void specialFunc(int key, int x, int y);

#endif //KALO_CONTROLLER_H
