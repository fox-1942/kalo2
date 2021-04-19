#include "camera.h"
#include <math.h>
#include <stdio.h>

#define SKYBOX_SIZE 6000
#define SUN_SIZE 920
#define JUPITER_SIZE 200
#define JUPITER_MOON_SIZE 85
#define VENUS_SIZE 220
#define SATURN_SIZE 150
#define SAT_SIZE 50
#define MOVE_SPEED 10.0  //BIGGER IS FASTER
#define CAMERA_SPEED 5.0 //BIGGER IS SLOWER

void init_camera(struct Camera *camera) {
    camera->position.x = 3000;
    camera->position.y = -5000;
    camera->position.z = 1000;

    camera->pose.x = -15;
    camera->pose.y = 0;
    camera->pose.z = 90;

    action.move_forward = FALSE;
    action.move_backward = FALSE;
    action.step_left = FALSE;
    action.step_right = FALSE;
    action.rotate_planets_in_galaxy = TRUE;
}

void update_camera_position(Camera *camera, Action *action, Move *move, GLfloat *light_ambient, double elapsed_time) {
    double distance;
    distance = elapsed_time * MOVE_SPEED * camera->camera_speed;

    if (action->move_forward == TRUE) {
        move_camera_forward(camera, distance);
    }

    if (action->move_backward == TRUE) {
        move_camera_backward(camera, distance);
    }

    if (action->step_left == TRUE) {
        step_camera_left(camera, distance);
    }

    if (action->step_right == TRUE) {
        step_camera_right(camera, distance);
    }

    if (action->move_up == TRUE) {
        move_camera_up(camera, distance);
    }

    if (action->move_down == TRUE) {
        move_camera_down(camera, distance);
    }

    if (action->increase_light == TRUE) {
        if (light_ambient[0] < 1)
            light_ambient[0] = light_ambient[1] = light_ambient[2] += 0.01;
    }

    if (action->decrease_light == TRUE) {
        if (light_ambient[0] > -0.51)
            light_ambient[0] = light_ambient[1] = light_ambient[2] -= 0.01;
    }

    don_not_head_up_against_the_wall(camera, move);
}

void don_not_head_up_against_the_wall(Camera *camera, Move *move) {

    if (camera->position.x < -SKYBOX_SIZE || camera->position.x > SKYBOX_SIZE ||
        camera->position.y < -SKYBOX_SIZE || camera->position.y > SKYBOX_SIZE ||
        camera->position.z < -SKYBOX_SIZE || camera->position.z > SKYBOX_SIZE)
        init_camera(camera);

    for (int i = 0; i < 6; ++i) {
        if (camera->position.x < move->planets[i]->x + world.planets[i]->size &&
            camera->position.x > move->planets[i]->x - world.planets[i]->size &&
            camera->position.y < move->planets[i]->y + world.planets[i]->size &&
            camera->position.y > move->planets[i]->y - world.planets[i]->size &&
            camera->position.z < move->planets[i]->z + world.planets[i]->size &&
            camera->position.z > move->planets[i]->z - world.planets[i]->size)
            init_camera(camera);
    }
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


