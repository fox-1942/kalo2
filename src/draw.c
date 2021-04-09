#include "draw.h"

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
    //draw_triangles(model);
    draw_quads(model);
    // draw_bounding_box(model);

}

void draw_skybox(Entity skybox, int z_sign) {
    double theta, phi1, phi2;
    double x1, y1, z1;
    double x2, y2, z2;
    double u, v1, v2;

    int n_slices, n_stacks;
    double radius;
    int i, k;

    n_slices = 12;
    n_stacks = 6;
    radius = SKYBOX_SIZE;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, skybox.texture);
    glScaled(radius, radius, radius);

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

double calc_elapsed_for_led() {
    int current_time;
    double elapsed_time;
    current_time = glutGet(GLUT_ELAPSED_TIME);
    elapsed_time = (double) current_time;
    return elapsed_time;
}

void draw_environment(World *world, Rotate *rotate, Move *move) {
    glEnable(GL_TEXTURE_2D);

    //Draw the bottom skybox.
    draw_skybox(world->skybox,0);

    //Draw the top skybox.
    draw_skybox(world->skybox,1);

    //Draw the sun.
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world->sun.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world->sun.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(rotate->sun_rotation, 1, 1, 1);
    draw_model(&world->sun.model);
    glPopMatrix();

    //Draw the planet1. Dark Jupiter
    glPushMatrix();
    glTranslatef(move->planet1.x, move->planet1.y, move->planet1.z);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world->planet1.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world->planet1.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(rotate->planet1_rotation, 0, 0, 1);
    draw_model(&world->planet1.model);

    glPopMatrix();


    //Draw the planet2, it is the moon of the dark Jupiter, so it is relative to the movement of planet 1.
    glPushMatrix();

    glTranslatef(move->planet1.x + 1000, move->planet1.y + 1000, move->planet1.z - 100);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world->planet2.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world->planet2.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(180, 0, 0, 1);
    glRotatef(rotate->planet2_rotation, 0, 0, 1);
    draw_model(&world->planet2.model);

    glPopMatrix();


    //Draw the planet3.   Light Jupiter
    glPushMatrix();
    glTranslatef(move->planet3.x, move->planet3.y, move->planet3.z);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world->planet3.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world->planet3.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(rotate->planet3_rotation, 0, 0, -1);
    draw_model(&world->planet3.model);
    glPopMatrix();

    //Draw the planet4. Saturnus
    glPushMatrix();
    glTranslatef(move->planet4.x, move->planet4.y, move->planet4.z);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world->planet4.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world->planet4.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(rotate->planet4_rotation, 0, 0, -1);
    draw_model(&world->planet4.model);
    glPopMatrix();

    //Draw the satellite.
    glPushMatrix();
    glTranslatef(move->satellite.x, move->satellite.y, move->satellite.z);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world->satellite.material_ambient);

    // switching texture if two seconds lasts
    // e_time variable is reset in change_satellite_texture. The delay
    // of resetting e_time means the blinked interval of the sat led.
    double r = calc_elapsed_for_led();
    if (r - e_time > 2000) {
        glBindTexture(GL_TEXTURE_2D, world->satellite.texture2);
    } else {
        glBindTexture(GL_TEXTURE_2D, world->satellite.texture);
    }

    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(90, 1, 0, 0);
    glRotatef(270, 0, 1, 0);
    glRotatef(rotate->satellite_rotation, 0, 0, 1);
    draw_model(&world->satellite.model);
    glPopMatrix();
}

// Determines whether the satellite is inside the planet's field of gravity. The radius is coming from
// the bounding box diagonal of the planet, calculated by the specific method in model.c
bool is_point_inside_spheres(double x, double y, double z, double x2, double y2, double z2, double radius) {

    /*printf("---------------------------------------------------------------------------\n");
      printf("Sat position: %f, %f, %f |",x, y, z);
      printf(" Planet position: %f, %f, %f",x2, y2, z2);

      printf("\nVector length: %f | radius: %f ",vector_length(x, y, z, x2, y2, z2), radius );
      if (vector_length(x, y, z, x2, y2, z2) < radius) {
          printf("--> Inside.\n");
      } else {
          printf("--> Outside.\n");
      }*/

    return vector_length(x, y, z, x2, y2, z2) < radius;
}

