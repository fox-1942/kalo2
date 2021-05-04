#include "scene.h"
#include <stdbool.h>
#include <math.h>

#define DIST_JUP 4000;
#define DIST_VENUS 6000;
#define DIST_SAT 2000;

void init_controller(Move *move) {
    Position *planetsToAdd[6] = {&move->jupiter, &move->jupiter_moon, &move->venus,
                                 &move->saturnus, &move->sun, &move->satellite};
    memcpy(move->planets, planetsToAdd, sizeof(planetsToAdd));
    for (int i = 0; i < 3; i++) { scene.action.light_ambient[i] = 0.5; }
}

void movement_of_objects(Move *move, Action *action, World *world) {
    bool inside_gravity_field = false;

    // Jupiter + its moon
    if (action->move_jupiter_plus_moon_in_galaxy == TRUE) {
        move->degree1 += 0.4;
        double angle = degree_to_radian(move->degree1);
        move->jupiter.x = cos(angle) * DIST_JUP;
        move->jupiter.y = sin(angle) * DIST_JUP;
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
        move->degree3 += 0.2;
        double angle = degree_to_radian(move->degree3);
        move->venus.x = cos(angle) * DIST_VENUS;
        move->venus.y = sin(angle) * DIST_VENUS;
        move->venus.z = 0;

    } else if (move->venus.x == 0) {
        move->venus.x = 6000;
        move->venus.y = 0;
        move->venus.z = 0;
    }

    // Saturnus
    if (action->move_saturnus_in_galaxy == TRUE) {
        move->degree4 += 0.3;
        double angle = degree_to_radian(move->degree4);
        move->saturnus.x = cos(angle) * DIST_SAT;
        move->saturnus.y = sin(angle) * DIST_SAT;
        move->saturnus.z = 0;

    } else if (move->saturnus.x == 0) {
        move->saturnus.x = 2000;
        move->saturnus.y = 0;
        move->saturnus.z = 0;
    }

    if (action->call_satellite == TRUE && move->satellite.x < 6000) {
        if (action->satellite_is_moving == 0) {
            move->satellite.x = -6000;
            move->satellite.y = 1500;
            move->satellite.z = 400;
        }

        // Examining, whether the sat is inside one of the gravity fields or not.
        int i;
        for (i = 0; i <= 4; i++) {
            if (is_point_inside_spheres(move->satellite.x, move->satellite.y, move->satellite.z,
                                        move->planets[i]->x, move->planets[i]->y, move->planets[i]->z,
                                        world->planets[i]->model.box.diagonal_length + 10)) {
                inside_gravity_field = true;
                break;
            }
        }

        if (inside_gravity_field) {
            Vertex distance_vector = vector_from_two_vertex(move->satellite.x, move->satellite.y, move->satellite.z,
                                                            move->planets[i]->x, move->planets[i]->y,
                                                            move->planets[i]->z);
            if (i != 4) { // every planet except Sun
                move->satellite.x += 1.0;

                if (distance_vector.y <= 0) {
                    move->satellite.y -= 1;
                } else {
                    move->satellite.y += 1;
                }
            } else { // Sun
                move->satellite.x += 5;
                move->satellite.y += 0.3;
            }

            inside_gravity_field = false;
        } else {
            move->satellite.x += 7;
        }

        action->satellite_is_moving = 1;

    } else if (action->call_satellite == TRUE && move->satellite.x >= 6000) {
        move->satellite.x = -20000;
        action->call_satellite = FALSE;
        action->satellite_is_moving = 0;
    } else if (action->call_satellite == FALSE) {
        move->satellite.x = -20000;
    }
}

void rotation_of_objects(Action *action, Rotate *rotate) {
    if (action->rotate_planets_in_galaxy == TRUE) {
        rotate->jupiter_rotation += 0.5;
        rotate->jupiter_moon_rotation += 1;
        rotate->venus_rotation += 0.4;
        rotate->saturnus_rotation += 0.2;
        rotate->sun_rotation += 0.05;
    }
    rotate->satellite_rotation += 0.5;
}

void specialFunc(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            if (scene.action.help_on) {
                scene.action.help_on = 0;
            } else {
                scene.action.help_on = 1;
            }
    }
}

void mouse_handler(int button, int state, int x, int y) {
    scene.window.mouse_x = x;
    scene.window.mouse_y = y;
}

void motion_handler(int x, int y) {
    double horizontal, vertical;

    horizontal = scene.window.mouse_x - x;
    vertical = scene.window.mouse_y - y;

    rotate_camera(&scene.camera, horizontal, vertical);

    scene.window.mouse_x = x;
    scene.window.mouse_y = y;

    glutPostRedisplay();
}

void key_handler(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            scene.camera.move_forward = TRUE;
            break;
        case 's':
            scene.camera.move_backward = TRUE;
            break;
        case 'a':
            scene.camera.step_left = TRUE;
            break;
        case 'd':
            scene.camera.step_right = TRUE;
            break;
        case 'c':
            scene.camera.move_down = TRUE;
            break;
        case 32:
            scene.camera.move_up = TRUE;
            break;
        case 'q':
            if (scene.action.rotate_planets_in_galaxy == FALSE) {
                scene.action.rotate_planets_in_galaxy = TRUE;
            } else {
                scene.action.rotate_planets_in_galaxy = FALSE;
            }
            break;
        case 'e':
            if (scene.action.move_jupiter_plus_moon_in_galaxy == FALSE) {
                scene.action.move_jupiter_plus_moon_in_galaxy = TRUE;
                scene.action.move_venus_in_galaxy = TRUE;
                scene.action.move_saturnus_in_galaxy = TRUE;
            } else {
                scene.action.move_jupiter_plus_moon_in_galaxy = FALSE;
                scene.action.move_venus_in_galaxy = FALSE;
                scene.action.move_saturnus_in_galaxy = FALSE;
            }
            break;
        case 'f':
            if (scene.action.call_satellite == FALSE) {
                scene.action.call_satellite = TRUE;
            }
            break;
        case 't':
            if (scene.action.fog == FALSE) {
                scene.action.fog = TRUE;
            } else {
                scene.action.fog = FALSE;
            }
        case '+':
            scene.action.increase_light = TRUE;
            break;
        case '-':
            scene.action.decrease_light = TRUE;
            break;
        case 27:
            exit(0);
    }

    glutPostRedisplay();
}

void key_up_handler(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            scene.camera.move_forward = FALSE;
            break;
        case 's':
            scene.camera.move_backward = FALSE;
            break;
        case 'a':
            scene.camera.step_left = FALSE;
            break;
        case 'd':
            scene.camera.step_right = FALSE;
            break;
        case 'c':
            scene.camera.move_down = FALSE;
            break;
        case 32:
            scene.camera.move_up = FALSE;
            break;
        case '+':
            scene.action.increase_light = FALSE;
            break;
        case '-':
            scene.action.decrease_light = FALSE;
            break;
        case 27:
            exit(0);
    }

    glutPostRedisplay();
}