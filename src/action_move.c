#include "action_move.h"

bool inside = false;
int satellite = 0;
double degree1 = 0;
double degree3 = 0;
double degree4 = 0;
double distance_a1 = 4000;
double distance_b1 = 4000;
double distance_a3 = 6000;
double distance_b3 = 6000;
double distance_a4 = 2000;
double distance_b4 = 2000;

void movement_of_objects(Move *move, Action* action, World* world) {

    // dark Jupiter + its moon
    if (action->move_planet1_in_galaxy == TRUE) {
        degree1 += 0.4;
        double angle = degree_to_radian(degree1);
        move->planet1.x = cos(angle) * distance_a1;
        move->planet1.y = sin(angle) * distance_b1;
        move->planet1.z = 0;
        move->planet2.x = move->planet1.x + 1000;
        move->planet2.y = move->planet1.y + 1000;
        move->planet2.z = move->planet1.z - 100;
    } else if (move->planet1.x == 0) {
        move->planet1.x = 4000;
        move->planet1.y = 0;
        move->planet1.z = 0;
        move->planet2.x = move->planet1.x + 1000;
        move->planet2.y = move->planet1.y + 1000;
        move->planet2.z = move->planet1.z - 100;
    }
    // Light jupiter
    if (action->move_planet3_in_galaxy == TRUE) {
        degree3 += 0.2;
        double angle = degree_to_radian(degree3);
        move->planet3.x = cos(angle) * distance_a3;
        move->planet3.y = sin(angle) * distance_b3;
        move->planet3.z = 0;

    } else if (move->planet3.x == 0) {
        move->planet3.x = 6000;
        move->planet3.y = 0;
        move->planet3.z = 0;
    }

    // Saturnus
    if (action->move_planet4_in_galaxy == TRUE) {
        degree4 += 0.3;
        double angle = degree_to_radian(degree4);
        move->planet4.x = cos(angle) * distance_a4;
        move->planet4.y = sin(angle) * distance_b4;
        move->planet4.z = 0;

    } else if (move->planet4.x == 0) {
        move->planet4.x = 2000;
        move->planet4.y = 0;
        move->planet4.z = 0;
    }

    if (action->call_satellite == TRUE && move->satellite.x < 6000) {
        if (satellite == 0) {
            move->satellite.x = -6000;
            move->satellite.y = 1500;
            move->satellite.z = 400;
        }

        // Examining, whether the sat is inside one of the gravity fields or not.
        int i;
        for (i = 0; i <= 4; i++) {
            if (is_point_inside_spheres(move->satellite.x, move->satellite.y, move->satellite.z,
                                        move->Move[i].x, move->Move[i].y, move->Move[i].z,
                                        world->World[i].model.box.diagonal_length + 10)) {
                inside = true;
                break;
            }
        }

        // If sat is inside on of the gravitiy fields.
        if (inside) {
            Vertex indicator = vector_from_two_vertex(move->satellite.x, move->satellite.y, move->satellite.z,
                                                      move->Move[i].x, move->Move[i].y, move->Move[i].z);

            // setting up the effects of gravity fields of the planets, taking the direction
            // where the satellite comes from also in consideration. Index of "i" indicates a specific planet.
            switch (i) {
                case 0:  // Planet 1 - Dark. Jupiter
                case 1:  // Planet 2 - Moon of Dark. Jupiter

                    if (indicator.y <= 0) {  // if satellite goes against the rotation of the first two planets
                        move->satellite.x += 1.0;  // satelitte losts more speed on the X axis than is the 'else' case
                        move->satellite.y -= 1;
                    } else {
                        move->satellite.x += 1.5; // if satellite goes parellel with the rotation of the first two planets
                        move->satellite.y += 1;
                    }
                    break;
                case 2: // Light Jupiter
                case 3: // Saturnus
                    if (indicator.y <= 0) {
                        move->satellite.x += 1.5;
                        move->satellite.y -= 1;
                    } else {
                        move->satellite.x += 1.0;
                        move->satellite.y += 1;
                    }
                    break;

                case 4: // Sun
                    move->satellite.x += 5;
                    move->satellite.y += 0.3;
                    break;
            }
            inside = false;  // It gives the possibility to let the satellite get the normal speed in the next iteration.
        } else {
            move->satellite.x += 7;
        }

        satellite = 1;

    } else if (action->call_satellite == TRUE && move->satellite.x >= 6000) {
        move->satellite.x = -20000;
        action->call_satellite = FALSE;
        satellite = 0;
    } else if (action->call_satellite == FALSE) {
        move->satellite.x = -20000;
    }
}