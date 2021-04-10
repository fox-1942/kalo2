#include "camera.h"

void init_camera(struct Camera *camera) {
    camera->position.x = 3000;
    camera->position.y = -5000;
    camera->position.z = 1000;

    camera->pose.x = -15;
    camera->pose.y = 0;
    camera->pose.z = 90;
}

void update_camera_position(Camera *camera, Action *action, Move *move, GLfloat *light_ambient, double elapsed_time,
                            double speed) {
    double distance;
    distance = elapsed_time * MOVE_SPEED * speed;

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

    //Skybox
    if (camera->position.x < -skybox_size || camera->position.x > skybox_size ||
        camera->position.y < -skybox_size || camera->position.y > skybox_size ||
        camera->position.z < -skybox_size || camera->position.z > skybox_size)
        init_camera(camera);
    //Sun
    if (camera->position.x < sun_size && camera->position.x > -sun_size &&
        camera->position.y < sun_size && camera->position.y > -sun_size &&
        camera->position.z < sun_size && camera->position.z > -sun_size)
        init_camera(camera);
    //Planet1
    if (camera->position.x < move->jupiter.x + planet1_size && camera->position.x > move->jupiter.x - planet1_size &&
        camera->position.y < move->jupiter.y + planet1_size && camera->position.y > move->jupiter.y - planet1_size &&
        camera->position.z < move->jupiter.z + planet1_size && camera->position.z > move->jupiter.z - planet1_size)
        init_camera(camera);
    //Planet2
    if (camera->position.x < move->jupiter.x + 1000 + planet2_size &&
        camera->position.x > move->jupiter.x + 1000 - planet2_size &&
        camera->position.y < move->jupiter.y + 1000 + planet2_size &&
        camera->position.y > move->jupiter.y + 1000 - planet2_size &&
        camera->position.z < move->jupiter.z - 100 + planet2_size &&
        camera->position.z > move->jupiter.z - 100 - planet2_size)
        init_camera(camera);
    //Planet3
    if (camera->position.x < move->venus.x + planet3_size && camera->position.x > move->venus.x - planet3_size &&
        camera->position.y < move->venus.y + planet3_size && camera->position.y > move->venus.y - planet3_size &&
        camera->position.z < move->venus.z + planet3_size && camera->position.z > move->venus.z - planet3_size)
        init_camera(camera);
    //Planet4
    if (camera->position.x < move->saturnus.x + planet4_size && camera->position.x > move->saturnus.x - planet4_size &&
        camera->position.y < move->saturnus.y + planet4_size && camera->position.y > move->saturnus.y - planet4_size &&
        camera->position.z < move->saturnus.z + planet4_size && camera->position.z > move->saturnus.z - planet4_size)
        init_camera(camera);
    //Satellite
    if (camera->position.x < move->satellite.x + satellite_size + 1500 &&
        camera->position.x > move->satellite.x - satellite_size &&
        camera->position.y < move->satellite.y + satellite_size &&
        camera->position.y > move->satellite.y - satellite_size &&
        camera->position.z < move->satellite.z + satellite_size &&
        camera->position.z > move->satellite.z - satellite_size)
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

    if (camera->pose.x < 0) {
        camera->pose.x += 360.0;
    }

    if (camera->pose.x > 360.0) {
        camera->pose.x -= 360.0;
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


