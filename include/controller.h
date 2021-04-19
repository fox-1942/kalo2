#ifndef KALO_CONTROLLER_H
#define KALO_CONTROLLER_H

#include "camera.h"
#include "draw.h"

#define RESOLUTION 0  // 1: 1920*1080   0: 1366*768
#define FULLSCREEN 0   // 1: fullscreen  0: windowed

typedef struct Data {
    int mouse_x;
    int mouse_y;
    int help, help_on;

    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
    double e_time;
    int previous_time;
} Data;

Data data;

void init_move(Move *move);

void movement_of_objects(Move *move, Action *action, World *world);

void rotation_of_objects(Action *action, Rotate *rotate);

void mouse_handler(int button, int state, int x, int y);

void motion_handler(int x, int y);

void key_handler(unsigned char key, int x, int y);

void key_up_handler(unsigned char key, int x, int y);

void specialFunc(int key, int x, int y);

#endif //KALO_CONTROLLER_H
