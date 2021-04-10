/* main.h is created for giving a better outlook of main.c*/

#ifndef KALO_MAIN_H
#define KALO_MAIN_H

#include "camera.h"

#define resolution 0  // 1: 1920*1080   0: 1366*768
#define fullscreen 0   // 1: fullscreen  0: windowed

GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 0 };
World world={0};
Rotate rotate={0};
Move move={0};
struct Camera camera={0};
struct Action action={0};

int WINDOW_WIDTH = 0;
int WINDOW_HEIGHT = 0;
int mouse_x = 0;
int mouse_y = 0;
int previous_time = 0;
int help, help_on = 0;
float speed = 70;
float angle = 135;


#endif //KALO_MAIN_H
