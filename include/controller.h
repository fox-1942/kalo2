#ifndef KALO_CONTROLLER_H
#define KALO_CONTROLLER_H

#include "camera.h"

#define resolution 0  // 1: 1920*1080   0: 1366*768
#define fullscreen 0   // 1: fullscreen  0: windowed

typedef struct Data {
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
    int mouse_x;
    int mouse_y;
    int previous_time;
    int help, help_on;
    float speed;
    float angle;
    double e_time;
} Data;

extern Data data;

void movement_of_objects(Move *move, Action *action, World *world);

void rotation_of_objects(Action *action, Rotate *rotate);

void mouse_handler(int button, int state, int x, int y);

void motion_handler(int x, int y);

void key_handler(unsigned char key, int x, int y);

void key_up_handler(unsigned char key, int x, int y);

void specialFunc(int key, int x, int y);

#endif //KALO_CONTROLLER_H
