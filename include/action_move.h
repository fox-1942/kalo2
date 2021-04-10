//
// Created by Fox-1942 on 10/04/2021.
//

#ifndef KALO_ACTION_MOVE_H
#define KALO_ACTION_MOVE_H

#include "draw.h"

typedef struct Action {
    int move_forward;
    int move_backward;
    int step_left;
    int step_right;
    int move_up;
    int move_down;
    int rotate_planet1_in_galaxy;
    int move_jupiter_plus_moon_in_galaxy;
    int move_venus_in_galaxy;
    int saturnus;
    int call_satellite;
    int increase_light;
    int decrease_light;
} Action;

void movement_of_objects(Move *move, Action *action, World *world);

void rotation_of_objects(Action *action, Rotate *rotate);

#endif //KALO_ACTION_MOVE_H
