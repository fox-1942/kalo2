#ifndef KALO_SCENE_H
#define KALO_SCENE_H

#include "basic_primitives.h"
#include "load_model.h"
#include "camera.h"
#include "callbacks.h"
#include "draw.h"
#include "model.h"

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
