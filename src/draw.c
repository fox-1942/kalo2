#include <controller.h>
#include <math.h>
#include "draw.h"

#define SKYBOX_SCALE 10000.0
GLfloat light_ambient[] = {0.5, 0.5, 0.5, 0};

void init_rotate(Rotate *rotate) {
    double *planetsToAdd[6] = {&rotate->jupiter_rotation, &rotate->jupiter_moon_rotation, &rotate->venus_rotation,
                               &rotate->saturnus_rotation, &rotate->sun_rotation, &rotate->satellite_rotation};
    memcpy(rotate->planets, planetsToAdd, sizeof(planetsToAdd));
}

void draw_quads(const struct Model *model) {
    int i, k;
    int vertex_index, texture_index;
    double x, y, z, u, v;

    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    for (i = 0; i < model->n_quads; ++i) {
        for (k = 0; k < 4; ++k) {

            texture_index = model->quads[i].points[k].texture_index;
            u = model->texture_vertices[texture_index].u;
            v = model->texture_vertices[texture_index].v;
            glTexCoord2f(u, 1 - v);

            vertex_index = model->quads[i].points[k].vertex_index;
            x = model->vertices[vertex_index].x;
            y = model->vertices[vertex_index].y;
            z = model->vertices[vertex_index].z;
            glVertex3d(x, y, z);
        }
    }
    glEnd();
}

void draw_bounding_box(const Model *model) {
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex3d(model->box.minVertex.x, model->box.minVertex.y, model->box.minVertex.z);
    glVertex3d(model->box.maxVertex.x, model->box.maxVertex.y, model->box.maxVertex.z);
    glEnd();
}

void draw_model(const Model *model) {
    draw_quads(model);
    // draw_bounding_box(model);
}

void draw_skybox(Entity skybox, int z_sign) {
    double theta, phi1, phi2;
    double x1, y1, z1;
    double x2, y2, z2;
    double u, v1, v2;

    int n_slices, n_stacks;

    int i, k;

    n_slices = 12;
    n_stacks = 6;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, skybox.texture);
    glScaled(SKYBOX_SCALE, SKYBOX_SCALE, SKYBOX_SCALE);

    glColor3f(1, 1, 1);

    glBegin(GL_TRIANGLE_STRIP);

    for (i = 0; i < n_stacks; ++i) {
        v1 = (double) i / n_stacks;
        v2 = (double) (i + 1) / n_stacks;
        phi1 = v1 * M_PI / 2.0;
        phi2 = v2 * M_PI / 2.0;
        for (k = 0; k <= n_slices; ++k) {
            u = (double) k / n_slices;
            theta = u * 2.0 * M_PI;
            x1 = cos(theta) * cos(phi1);
            y1 = sin(theta) * cos(phi1);
            z1 = sin(phi1);
            x2 = cos(theta) * cos(phi2);
            y2 = sin(theta) * cos(phi2);
            z2 = sin(phi2);

            if (z_sign == 0) {
                glTexCoord2d(u, 1.0 - v1);
                glVertex3d(x1, y1, z1);
                glTexCoord2d(u, 1.0 - v2);
                glVertex3d(x2, y2, z2);
            } else {
                glTexCoord2d(u, 1.0 - v1);
                glVertex3d(x1, y1, -z1);
                glTexCoord2d(u, 1.0 - v2);
                glVertex3d(x2, y2, -z2);
            }
        }
    }
    glEnd();
    glPopMatrix();
}

void draw_environment(World *world, Rotate *rotate, Move *move, double timer) {
    glEnable(GL_TEXTURE_2D);

    for (int i = 0; i < 6; ++i) {
        glPushMatrix();
        glTranslatef(move->planets[i]->x, move->planets[i]->y, move->planets[i]->z);
        glMaterialfv(GL_FRONT, GL_AMBIENT, world->planets[i]->material_ambient);

        if (i == 5) {  // Switching textures in case of satellite.
            double actual_t = (double) glutGet(GLUT_ELAPSED_TIME);
            if (actual_t - timer > 2000) {
                glBindTexture(GL_TEXTURE_2D, world->planets[i]->texture2);
            } else {
                glBindTexture(GL_TEXTURE_2D, world->planets[i]->texture);
            }
        } else {
            glBindTexture(GL_TEXTURE_2D, world->planets[i]->texture);
        }

        glScalef(1.0f, 1.0f, 1.0f);

        if (i == 0 || i == 1) {
            glRotatef(*rotate->planets[i], 0, 0, 1);  // Jupiter and its moon
        } else if (i == 2 || i == 3) {
            glRotatef(*rotate->planets[i], 0, 0, -1); // Venus, Saturnus
        } else if (i == 4) {
            glRotatef(*rotate->planets[i], 1, 1, 1);  //Sun
        } else if (i == 5) {                                 // Satellite
            glRotatef(90, 1, 0, 0);
            glRotatef(270, 0, 1, 0);
            glRotatef(*rotate->planets[i], 0, 0, 1);
        }

        draw_model(&world->planets[i]->model);
        glPopMatrix();
    }

    draw_skybox(world->skybox, 0);
    draw_skybox(world->skybox, 1);
}

// Determines whether the satellite is inside the planet's field of gravity. The radius is coming from
// the bounding box diagonal of the planet, calculated by the specific method in model.c
bool is_point_inside_spheres(double x, double y, double z, double x2, double y2, double z2, double radius) {
    return vector_length(x, y, z, x2, y2, z2) < radius;
}

void reshape(GLsizei width, GLsizei height) {
    window.window_width = width;
    window.window_height = height;

    glViewport(0, (height - 768) / 2, width, 768);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (!action.help_on) {
        gluPerspective(50.0, (GLdouble) width / (GLdouble) 768, 0.1, 20000.0);
    } else {
        gluOrtho2D(0, width, height, 0);
    }
}

void draw_help() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, action.help);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);
    glTexCoord2f(1, 0);
    glVertex3f(window.window_width, 0, 0);
    glTexCoord2f(1, 1);
    glVertex3f(window.window_width, window.window_height, 0);
    glTexCoord2f(0, 1);
    glVertex3f(0, window.window_height, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    reshape(window.window_width, window.window_height);
    glutSwapBuffers();
}

// The delay of resetting e_time means the time of the led operation on the sat.
// Texture change in the draw_environment() method.
void set_satellite_led_working_time() {
    window.e_time = (double) glutGet(GLUT_ELAPSED_TIME);
    glutTimerFunc(3000, set_satellite_led_working_time, 0);
}

void display() {
    if (!action.help_on) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        update_camera_position(&camera, &move);
        set_view_point(&camera);

        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
        glEnable(GL_LIGHT1);

        draw_environment(&world, &rotate, &move, window.e_time);
        movement_of_objects(&move, &action, &world);
        rotation_of_objects(&action, &rotate);
        reshape(window.window_width, window.window_height);
        glutSwapBuffers();
    } else {
        draw_help();
    }

    if (action.increase_light == TRUE) {
        if (light_ambient[0] < 1)
            light_ambient[0] = light_ambient[1] = light_ambient[2] += 0.01;
    }

    if (action.decrease_light == TRUE) {
        if (light_ambient[0] > -0.51)
            light_ambient[0] = light_ambient[1] = light_ambient[2] -= 0.01;
    }
}

void idle() {
    glutPostRedisplay();
}
