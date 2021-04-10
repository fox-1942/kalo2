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
Camera camera={0};
Action action={0};
Data data={0};

#endif //KALO_MAIN_H
