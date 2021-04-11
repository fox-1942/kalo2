#ifndef KALO_CONTROLLER_H
#define KALO_CONTROLLER_H

#include "planet_controller.h"

#define resolution 0  // 1: 1920*1080   0: 1366*768
#define fullscreen 0   // 1: fullscreen  0: windowed

void mouse_handler(int button, int state, int x, int y);

void motion_handler(int x, int y);

void key_handler(unsigned char key, int x, int y);

void key_up_handler(unsigned char key, int x, int y);

void specialFunc(int key, int x, int y);




#endif //KALO_CONTROLLER_H
