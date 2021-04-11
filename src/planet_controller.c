#include "planet_controller.h"

bool inside_gravity_field = false;
int satellite;
double degree1;
double degree3;
double degree4;
const double distance_jupiter_a = 4000;
const double distance_jupiter_b = 4000;
const double distance_venus_a = 6000;
const double distance_venus_b = 6000;
const double distance_saturn_a = 2000;
const double distance_dsturn_b = 2000;

void movement_of_objects(Move *move, Action *action, World *world) {

    // Jupiter + its moon
    if (action->move_jupiter_plus_moon_in_galaxy == TRUE) {
        degree1 += 0.4;
        double angle = degree_to_radian(degree1);
        move->jupiter.x = cos(angle) * distance_jupiter_a;
        move->jupiter.y = sin(angle) * distance_jupiter_b;
        move->jupiter.z = 0;
        move->jupiter_moon.x = move->jupiter.x + 1000;
        move->jupiter_moon.y = move->jupiter.y + 1000;
        move->jupiter_moon.z = move->jupiter.z - 100;
    } else if (move->jupiter.x == 0) {
        move->jupiter.x = 4000;
        move->jupiter.y = 0;
        move->jupiter.z = 0;
        move->jupiter_moon.x = move->jupiter.x + 1000;
        move->jupiter_moon.y = move->jupiter.y + 1000;
        move->jupiter_moon.z = move->jupiter.z - 100;
    }

    // Venus
    if (action->move_venus_in_galaxy == TRUE) {
        degree3 += 0.2;
        double angle = degree_to_radian(degree3);
        move->venus.x = cos(angle) * distance_venus_a;
        move->venus.y = sin(angle) * distance_venus_b;
        move->venus.z = 0;

    } else if (move->venus.x == 0) {
        move->venus.x = 6000;
        move->venus.y = 0;
        move->venus.z = 0;
    }

    // Saturnus
    if (action->saturnus == TRUE) {
        degree4 += 0.3;
        double angle = degree_to_radian(degree4);
        move->saturnus.x = cos(angle) * distance_saturn_a;
        move->saturnus.y = sin(angle) * distance_dsturn_b;
        move->saturnus.z = 0;

    } else if (move->saturnus.x == 0) {
        move->saturnus.x = 2000;
        move->saturnus.y = 0;
        move->saturnus.z = 0;
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
                inside_gravity_field = true;
                break;
            }
        }

        // If sat is inside on of the gravitiy fields.
        if (inside_gravity_field) {
            Vertex distance_vector = vector_from_two_vertex(move->satellite.x, move->satellite.y, move->satellite.z,
                                                            move->Move[i].x, move->Move[i].y, move->Move[i].z);

            // setting up the effects of gravity fields of the planets, taking the direction
            // where the satellite comes from also in consideration. Index of "i" indicates a specific planet.
            switch (i) {
                case 0:  // Planet 1 - Dark. Jupiter
                case 1:  // Planet 2 - Moon of Dark. Jupiter

                    if (distance_vector.y <= 0) {  // if satellite goes against the rotation of the first two planets
                        move->satellite.x += 1.0;  // satelitte losts more speed on the X axis than is the 'else' case
                        move->satellite.y -= 1;
                    } else {
                        move->satellite.x += 1.5; // if satellite goes parellel with the rotation of the first two planets
                        move->satellite.y += 1;
                    }
                    break;
                case 2: // Light Jupiter
                case 3: // Saturnus
                    if (distance_vector.y <= 0) {
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
            inside_gravity_field = false;  // It gives the possibility to let the satellite get the normal speed in the next iteration.
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

void rotation_of_objects(Action* action, Rotate* rotate) {
    if (action->rotate_planet1_in_galaxy == TRUE) {
        rotate->planet1_rotation += 0.5;
        rotate->planet2_rotation += 1;
        rotate->planet3_rotation += 0.4;
        rotate->planet4_rotation += 0.2;
        rotate->sun_rotation += 0.05;
    }
    rotate->satellite_rotation += 0.5;
}