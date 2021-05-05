#include "scene.h"
#include <math.h>

#define MOVE_SPEED 10.0  //BIGGER IS FASTER
#define CAMERA_SPEED 5.0 //BIGGER IS SLOWER

void init_camera(struct Camera *camera) {
    camera->position.x = 3000;
    camera->position.y = -5000;
    camera->position.z = 1000;

    camera->pose.x = -15;
    camera->pose.y = 0;
    camera->pose.z = 90;

    camera->move_forward = FALSE;
    camera->move_backward = FALSE;
    camera->step_left = FALSE;
    camera->step_right = FALSE;
}

double calc_elapsed_time() {
    int current_time;
    double elapsed_time;
    current_time = glutGet(GLUT_ELAPSED_TIME);
    elapsed_time = (double) (current_time - scene.window.previous_time) / 1000.0;
    scene.window.previous_time = current_time;
    return elapsed_time;
}

void update_camera_position(Camera *camera, Move *move) {
    double distance = calc_elapsed_time() * MOVE_SPEED * camera->camera_speed;

    if (camera->move_forward == TRUE) {
        move_camera_forward(camera, distance);
    }

    if (camera->move_backward == TRUE) {
        move_camera_backward(camera, distance);
    }

    if (camera->step_left == TRUE) {
        step_camera_left(camera, distance);
    }

    if (camera->step_right == TRUE) {
        step_camera_right(camera, distance);
    }

    if (camera->move_up == TRUE) {
        move_camera_up(camera, distance);
    }

    if (camera->move_down == TRUE) {
        move_camera_down(camera, distance);
    }
    don_not_head_up_against_the_wall(camera, move);
}

void don_not_head_up_against_the_wall(Camera *camera, Move *move) {

    for (int i = 0; i < 6; ++i) {
        if (camera->position.x < move->planets[i]->x + scene.world.planets[i]->size &&
            camera->position.x > move->planets[i]->x - scene.world.planets[i]->size &&
            camera->position.y < move->planets[i]->y + scene.world.planets[i]->size &&
            camera->position.y > move->planets[i]->y - scene.world.planets[i]->size &&
            camera->position.z < move->planets[i]->z + scene.world.planets[i]->size &&
            camera->position.z > move->planets[i]->z - scene.world.planets[i]->size)
            init_camera(camera);
    }

    // Skybox
    if (camera->position.x < -scene.world.planets[6]->size || camera->position.x > scene.world.planets[6]->size ||
        camera->position.y < -scene.world.planets[6]->size || camera->position.y > scene.world.planets[6]->size ||
        camera->position.z < -scene.world.planets[6]->size || camera->position.z > scene.world.planets[6]->size)
        init_camera(camera);
}

void set_view_point(const struct Camera *camera) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-(camera->pose.x + 90), 1.0, 0, 0);
    glRotatef(-(camera->pose.z - 90), 0, 0, 1.0);
    glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);
}

void rotate_camera(struct Camera *camera, double horizontal, double vertical) {
    camera->pose.z += horizontal / CAMERA_SPEED;
    camera->pose.x += vertical / CAMERA_SPEED;

    if (camera->pose.z < 0) {
        camera->pose.z += 360.0;
    }

    if (camera->pose.z > 360.0) {
        camera->pose.z -= 360.0;
    }

    if (camera->pose.x < -90) {
        camera->pose.x += 90.0;
    }

    if (camera->pose.x > 50.0) {
        camera->pose.x -= 50.0;
    }

}

void move_camera_up(struct Camera *camera, double distance) {
    camera->prev_position = camera->position;
    camera->position.z += distance;

}

void move_camera_down(struct Camera *camera, double distance) {
    camera->prev_position = camera->position;
    camera->position.z -= distance;
}

void move_camera_backward(struct Camera *camera, double distance) {
    camera->prev_position = camera->position;
    double angle = degree_to_radian(camera->pose.z);

    camera->position.x -= cos(angle) * distance;
    camera->position.y -= sin(angle) * distance;
}

void move_camera_forward(struct Camera *camera, double distance) {
    camera->prev_position = camera->position;
    double angle = degree_to_radian(camera->pose.z);
    camera->position.x += cos(angle) * distance;
    camera->position.y += sin(angle) * distance;
}

void step_camera_right(struct Camera *camera, double distance) {
    camera->prev_position = camera->position;
    double angle = degree_to_radian(camera->pose.z + 90.0);
    camera->position.x -= cos(angle) * distance;
    camera->position.y -= sin(angle) * distance;
}

void step_camera_left(struct Camera *camera, double distance) {
    camera->prev_position = camera->position;
    double angle = degree_to_radian(camera->pose.z - 90.0);

    camera->position.x -= cos(angle) * distance;
    camera->position.y -= sin(angle) * distance;
}
