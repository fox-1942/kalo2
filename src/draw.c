#include "scene.h"
#include <math.h>

#define SKYBOX_SCALE 10000.0

void init_draw(Rotate *rotate) {
    double *planetsToAdd[6] = {&rotate->jupiter_rotation, &rotate->jupiter_moon_rotation, &rotate->venus_rotation,
                               &rotate->saturnus_rotation, &rotate->sun_rotation, &rotate->satellite_rotation};
    memcpy(rotate->planets, planetsToAdd, sizeof(planetsToAdd));
}

void draw_triangles(const Model *model) {
    int i, k;
    int vertex_index, texture_index, normal_index;
    float x, y, z, u, v;

    glBegin(GL_TRIANGLES);

    for (i = 0; i < model->n_triangles; ++i) {
        for (k = 0; k < 3; ++k) {

            normal_index = model->triangles[i].points[k].normal_index;
            x = model->normals[normal_index].x;
            y = model->normals[normal_index].y;
            z = model->normals[normal_index].z;
            glNormal3f(x, y, z);

            texture_index = model->triangles[i].points[k].texture_index;
            u = model->texture_vertices[texture_index].u;
            v = model->texture_vertices[texture_index].v;
            glTexCoord2f(u, 1.0 - v);

            vertex_index = model->triangles[i].points[k].vertex_index;
            x = model->vertices[vertex_index].x;
            y = model->vertices[vertex_index].y;
            z = model->vertices[vertex_index].z;
            glVertex3f(x, y, z);
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
    draw_triangles(model);
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
        glScalef(1.0f, 1.0f, 1.0f);

        if (i == 5) {  // Switching textures in case of satellite.
            for (int j = 0; j < 3; j++) {
                glTranslatef(move->satellite[j].x, move->satellite[j].y, move->satellite[j].z);
                glMaterialfv(GL_FRONT, GL_AMBIENT, world->satellite[j].material_ambient);

                double actual_t = (double) glutGet(GLUT_ELAPSED_TIME);
                if (actual_t - timer > 2000) {
                    glBindTexture(GL_TEXTURE_2D, world->satellite[j].texture2);
                } else {
                    glBindTexture(GL_TEXTURE_2D, world->satellite[j].texture);
                }

                // glRotatef(90, 1, 0, 0);
                // glRotatef(270, 0, 1, 0);
                // glRotatef(*rotate->planets[i], 0, 0, 1);

                draw_model(&(world->satellite[j]).model);
            }

        } else {
            glTranslatef(move->planets[i]->x, move->planets[i]->y, move->planets[i]->z);
            glMaterialfv(GL_FRONT, GL_AMBIENT, world->planets[i]->material_ambient);
            glBindTexture(GL_TEXTURE_2D, world->planets[i]->texture);

            if (i == 0 || i == 1) {                                 // Jupiter and its moon
                glRotatef(*rotate->planets[i], 0, 0, 1);
            } else if (i == 2 || i == 3) {                          // Venus, Saturnus
                glRotatef(*rotate->planets[i], 0, 0, -1);
            } else if (i == 4) {                                    //Sun
                glRotatef(*rotate->planets[i], 1, 1, 1);
            }
            draw_model(&world->planets[i]->model);
        }
        glPopMatrix();
    }

    draw_skybox(world->skybox, 0);
    draw_skybox(world->skybox, 1);
}

bool is_point_inside_spheres(double x, double y, double z, double x2, double y2, double z2, double radius) {
    return vector_length(x, y, z, x2, y2, z2) < radius;
}

void reshape(GLsizei width, GLsizei height) {
    scene.window.width = width;
    scene.window.height = height;

    double ratio = (double) width / height;
    int w_depend_on_h, h_depend_on_w;
    if (ratio > 1.77) {
        w_depend_on_h = (int) ((double) height * 1.77);
        glViewport((width - w_depend_on_h) / 2, 0, w_depend_on_h, height);
    } else {
        h_depend_on_w = (int) ((double) width / 1.77);
        glViewport(0, (height - h_depend_on_w) / 2, width, h_depend_on_w);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (!scene.action.help_on) {
        gluPerspective(50.0, (GLdouble) width / ((GLdouble) width / 1.77), 0.1, 20000.0);
    } else {
        gluOrtho2D(0, width, height, 0);
    }
}

void draw_help() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, scene.action.help);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);
    glTexCoord2f(1, 0);
    glVertex3f(scene.window.width, 0, 0);
    glTexCoord2f(1, 1);
    glVertex3f(scene.window.width, scene.window.height, 0);
    glTexCoord2f(0, 1);
    glVertex3f(0, scene.window.height, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    reshape(scene.window.width, scene.window.height);
    glutSwapBuffers();
}

// The delay of resetting e_time means the time of the led operation on the sat.
// Texture change in the draw_environment() method.
void set_satellite_led_working_time() {
    scene.window.e_time = (double) glutGet(GLUT_ELAPSED_TIME);
    glutTimerFunc(3000, set_satellite_led_working_time, 0);
}

void display() {
    if (!scene.action.help_on) {
        glClearColor(0.5, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        update_camera_position(&scene.camera, &scene.move);
        set_view_point(&scene.camera);

        glLightfv(GL_LIGHT1, GL_AMBIENT, scene.action.light_ambient);
        glEnable(GL_LIGHT1);

        draw_environment(&scene.world, &scene.rotate, &scene.move, scene.window.e_time);
        movement_of_objects(&scene.move, &scene.action, &scene.world);
        rotation_of_objects(&scene.action, &scene.rotate);
        reshape(scene.window.width, scene.window.height);
        glutSwapBuffers();
    } else {
        draw_help();
    }

    if (scene.action.increase_light == TRUE) {
        if (scene.action.light_ambient[0] < 1)
            scene.action.light_ambient[0] = scene.action.light_ambient[1] = scene.action.light_ambient[2] += 0.01;
    }

    if (scene.action.decrease_light == TRUE) {
        if (scene.action.light_ambient[0] > -0.51)
            scene.action.light_ambient[0] = scene.action.light_ambient[1] = scene.action.light_ambient[2] -= 0.01;
    }

    if (scene.action.fog) {
        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, GL_EXP);
        glHint(GL_FOG_HINT, GL_NICEST);
        glFogf(GL_FOG_DENSITY, 0.00006);
        float color[] = {0.34, 0.36, 0.88, 1.0};
        glFogfv(GL_FOG_COLOR, color);
    } else {
        glDisable(GL_FOG);
    }
}

void idle() {
    glutPostRedisplay();
}