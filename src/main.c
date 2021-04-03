#include "main.h"

double calc_elapsed_time() {
    int current_time;
    double elapsed_time;

    current_time = glutGet(GLUT_ELAPSED_TIME);
    elapsed_time = (double) (current_time - previous_time) / 1000.0;
    previous_time = current_time;

    return elapsed_time;
}

void update_camera_position(struct Camera *camera, double elapsed_time) {
    double distance;

    distance = elapsed_time * MOVE_SPEED * speed;

    if (action.move_forward == TRUE) {
        move_camera_forward(camera, distance);
    }

    if (action.move_backward == TRUE) {
        move_camera_backward(camera, distance);
    }

    if (action.step_left == TRUE) {
        step_camera_left(camera, distance);
    }

    if (action.step_right == TRUE) {
        step_camera_right(camera, distance);
    }

    if (action.move_up == TRUE) {
        move_camera_up(camera, distance);
    }

    if (action.move_down == TRUE) {
        move_camera_down(camera, distance);
    }

    if (action.increase_light == TRUE) {
        if (light_ambient[0] < 1)
            light_ambient[0] = light_ambient[1] = light_ambient[2] += 0.01;
    }

    if (action.decrease_light == TRUE) {
        if (light_ambient[0] > -0.51)
            light_ambient[0] = light_ambient[1] = light_ambient[2] -= 0.01;
    }

    don_not_head_up_against_the_wall(camera, move);

}

void specialFunc(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            if (help_on) {
                help_on = 0;

                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light_ambient);
                glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
            } else {
                help_on = 1;
                GLfloat ones[] = {1, 1, 1, 1};
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ones);
                glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ones);
            }
    }

}

void reshape(GLsizei width, GLsizei height) {
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (!help_on)
        gluPerspective(50.0, (GLdouble) width / (GLdouble) height, 0.1, 20000.0);
    else
        gluOrtho2D(0, width, height, 0);
}

void draw_help() {

    GLfloat ones[] = {1, 1, 1, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ones);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ones);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, help);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);
    glTexCoord2f(1, 0);
    glVertex3f(WINDOW_WIDTH, 0, 0);
    glTexCoord2f(1, 1);
    glVertex3f(WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    glTexCoord2f(0, 1);
    glVertex3f(0, WINDOW_HEIGHT, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutSwapBuffers();
}


void rotation_of_objects() {
    if (action.rotate_planet1_in_galaxy == TRUE) {
        rotate.planet1_rotation += 0.5;
        rotate.planet2_rotation += 1;
        rotate.planet3_rotation += 0.4;
        rotate.planet4_rotation += 0.2;
        rotate.sun_rotation += 0.05;
    }
    rotate.satellite_rotation += 0.5;
}

void movement_of_objects() {

    // dark Jupiter +its moon
    if (action.move_planet1_in_galaxy == TRUE) {
        degree1 += 0.4;
        double angle = degree_to_radian(degree1);
        move.planet1.x = cos(angle) * distance_a1;
        move.planet1.y = sin(angle) * distance_b1;
        move.planet1.z = 500;
        move.planet2.x=move.planet1.x + 1000;
        move.planet2.y=move.planet1.y + 1000;
        move.planet2.z=move.planet1.z - 100;
    } else if (move.planet1.x == 0) {
        move.planet1.x = 4000;
        move.planet1.y = 0;
        move.planet1.z = 500;
        move.planet2.x=move.planet1.x + 1000;
        move.planet2.y=move.planet1.y + 1000;
        move.planet2.z=move.planet1.z - 100;
    }

    // Light jupiter
    if (action.move_planet3_in_galaxy == TRUE) {
        degree3 += 0.2;
        double angle = degree_to_radian(degree3);
        move.planet3.x = cos(angle) * distance_a3;
        move.planet3.y = sin(angle) * distance_b3;
        move.planet3.z = 0;

    } else if (move.planet3.x == 0) {
        move.planet3.x = 6000;
        move.planet3.y = 0;
        move.planet3.z = 0;
    }

    // Saturnus
    if (action.move_planet4_in_galaxy == TRUE) {
        degree4 += 0.3;
        double angle = degree_to_radian(degree4);
        move.planet4.x = cos(angle) * distance_a4;
        move.planet4.y = sin(angle) * distance_b4;
        move.planet4.z = 0;

    } else if (move.planet4.x == 0) {
        move.planet4.x = 2000;
        move.planet4.y = 0;
        move.planet4.z = 0;
    }

    if (action.call_satellite == TRUE && move.satellite.x < 6000) {
        if (satellite == 0) {
            move.satellite.x = -6000;
            move.satellite.y = 1500;
            move.satellite.z = 400;
        }

        bool inside = is_point_inside_spheres(move.satellite.x, move.satellite.y, move.satellite.z,
                                              move.planet2.x, move.planet2.y, move.planet2.z,
                                              world.planet2.model.box.diagonal_length);
        if (inside) {
            move.satellite.x += 0;

        } else {
            move.satellite.x += 10;
        }

        satellite = 1;

    } else if (action.call_satellite == TRUE && move.satellite.x >= 6000) {
        move.satellite.x = -20000;
        action.call_satellite = FALSE;
        satellite = 0;
    } else if (action.call_satellite == FALSE) {
        move.satellite.x = -20000;
    }
}


void display() {
    if (!help_on) {
        double elapsed_time;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        elapsed_time = calc_elapsed_time();
        update_camera_position(&camera, elapsed_time);
        set_view_point(&camera);

        glLightfv(GL_LIGHT1, GL_POSITION, light_position);
        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, light_ambient);
        glEnable(GL_LIGHT1);

        draw_environment(world, &rotate, move);
        movement_of_objects();
        rotation_of_objects();
        reshape(WINDOW_WIDTH, WINDOW_HEIGHT);


        glutSwapBuffers();

    } else {
        draw_help();
    }
}

void mouse_handler(int button, int state, int x, int y) {
    mouse_x = x;
    mouse_y = y;
}

void motion_handler(int x, int y) {
    double horizontal, vertical;

    horizontal = mouse_x - x;
    vertical = mouse_y - y;

    rotate_camera(&camera, horizontal, vertical);

    mouse_x = x;
    mouse_y = y;

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
                action.rotate_planet2_in_galaxy = TRUE;
                action.rotate_planet3_in_galaxy = TRUE;
                action.rotate_planet4_in_galaxy = TRUE;
            } else {
                action.rotate_planet1_in_galaxy = FALSE;
                action.rotate_planet2_in_galaxy = FALSE;
                action.rotate_planet3_in_galaxy = FALSE;
                action.rotate_planet4_in_galaxy = FALSE;
            }
            break;
        case 'e':
            if (action.move_planet1_in_galaxy == FALSE) {
                action.move_planet1_in_galaxy = TRUE;
                action.move_planet3_in_galaxy = TRUE;
                action.move_planet4_in_galaxy = TRUE;
            } else {
                action.move_planet1_in_galaxy = FALSE;
                action.move_planet3_in_galaxy = FALSE;
                action.move_planet4_in_galaxy = FALSE;
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


void idle() {
    glutPostRedisplay();
}

void set_callbacks() {
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key_handler);
    glutKeyboardUpFunc(key_up_handler);
    glutMouseFunc(mouse_handler);
    glutMotionFunc(motion_handler);
    glutIdleFunc(idle);
    glutSpecialFunc(specialFunc);

    if (fullscreen == 1)
        glutFullScreen();
}


void initialize() {
    set_callbacks();

    init_camera(&camera);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);
    glClearColor(0.1, 0.1, 0.1, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glClearDepth(1.0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light_ambient);
    glEnable(GL_LIGHTING);
    help = load_texture("..\\textures\\help.png");
    init_entities(&world);
    glEnable(GL_TEXTURE_2D);
}


int main(int argc, char **argv) {

    glutInit(&argc, argv);
    if (resolution == 1) { glutInitWindowSize(1920, 1080); }
    else { glutInitWindowSize(1366, 768); }
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    int wnd = glutCreateWindow("New Solar System");
    glutSetWindow(wnd);

    initialize();

    action.move_forward = FALSE;
    action.move_backward = FALSE;
    action.step_left = FALSE;
    action.step_right = FALSE;
    action.rotate_planet1_in_galaxy = TRUE;
    action.rotate_planet2_in_galaxy = TRUE;
    action.rotate_planet3_in_galaxy = TRUE;
    action.rotate_planet4_in_galaxy = TRUE;
    action.call_satellite = FALSE;

    glutMainLoop();
    return 0;
}
