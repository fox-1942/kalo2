//
// Created by Fox-1942 on 10/04/2021.
//

#ifndef KALO_PLANET_CONTROLLER_H
#define KALO_PLANET_CONTROLLER_H

#include "camera.h"

void movement_of_objects(Move *move, Action *action, World *world);

void rotation_of_objects(Action *action, Rotate *rotate);

#endif //KALO_PLANET_CONTROLLER_H
