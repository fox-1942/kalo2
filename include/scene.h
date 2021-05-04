//
// Created by Fox-1942 on 04/05/2021.
//

#ifndef KALO_SCENE_H
#define KALO_SCENE_H

#include "load_model.h"
#include "controller.h"

typedef struct Scene{
    Camera camera;
    Window window;
    Action action;
    Rotate rotate;
    Move move;
    World world;
} Scene;

Scene scene;

#endif //KALO_SCENE_H
