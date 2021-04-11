#ifndef KALO_CONTROLLER_H
#define KALO_CONTROLLER_H

#include "camera.h"

#define resolution 0  // 1: 1920*1080   0: 1366*768
#define fullscreen 0   // 1: fullscreen  0: windowed

extern World world;
extern Rotate rotate;
extern Move move;
extern Camera camera;
extern Action action;
extern Data data;

void display();

void mouse_handler(int button, int state, int x, int y);

void motion_handler(int x, int y);

void key_handler(unsigned char key, int x, int y);

void key_up_handler(unsigned char key, int x, int y);

void idle();

void reshape(GLsizei width, GLsizei height);

void specialFunc(int key, int x, int y);

void set_satellite_led_working_time();

#endif //KALO_CONTROLLER_H
