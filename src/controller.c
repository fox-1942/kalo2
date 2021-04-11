#include "controller.h"

void specialFunc(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            if (data.help_on) {
                data.help_on = 0;
            } else {
                data.help_on = 1;
            }
    }
}

void mouse_handler(int button, int state, int x, int y) {
    data.mouse_x = x;
    data.mouse_y = y;
}

void motion_handler(int x, int y) {
    double horizontal, vertical;

    horizontal = data.mouse_x - x;
    vertical = data.mouse_y - y;

    rotate_camera(&camera, horizontal, vertical);

    data.mouse_x = x;
    data.mouse_y = y;

    glutPostRedisplay();
}

void key_handler(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            action.move_forward = TRUE;
            break;
        case 's':
            action.move_backward = TRUE;
            break;
        case 'a':
            action.step_left = TRUE;
            break;
        case 'd':
            action.step_right = TRUE;
            break;
        case 'c':
            action.move_down = TRUE;
            break;
        case 32:
            action.move_up = TRUE;
            break;
        case 'q':
            if (action.rotate_planet1_in_galaxy == FALSE) {
                action.rotate_planet1_in_galaxy = TRUE;
            } else {
                action.rotate_planet1_in_galaxy = FALSE;
            }
            break;
        case 'e':
            if (action.move_jupiter_plus_moon_in_galaxy == FALSE) {
                action.move_jupiter_plus_moon_in_galaxy = TRUE;
                action.move_venus_in_galaxy = TRUE;
                action.saturnus = TRUE;
            } else {
                action.move_jupiter_plus_moon_in_galaxy = FALSE;
                action.move_venus_in_galaxy = FALSE;
                action.saturnus = FALSE;
            }
            break;
        case 'f':
            if (action.call_satellite == FALSE) {
                action.call_satellite = TRUE;
            }
            break;
        case '+':
            action.increase_light = TRUE;
            break;
        case '-':
            action.decrease_light = TRUE;
            break;
        case 27:
            exit(0);
    }

    glutPostRedisplay();
}

void key_up_handler(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            action.move_forward = FALSE;
            break;
        case 's':
            action.move_backward = FALSE;
            break;
        case 'a':
            action.step_left = FALSE;
            break;
        case 'd':
            action.step_right = FALSE;
            break;
        case 'c':
            action.move_down = FALSE;
            break;
        case 32:
            action.move_up = FALSE;
            break;
        case '+':
            action.increase_light = FALSE;
            break;
        case '-':
            action.decrease_light = FALSE;
            break;
        case 27:
            exit(0);
    }

    glutPostRedisplay();
}





