#include "draw.h"

void draw_triangles(const struct Model *model) {
    int i, k;
    int vertex_index, normal_index;
    double x, y, z, normal_x, normal_y, normal_z;

    glBegin(GL_TRIANGLES);

    for (i = 0; i < model->n_triangles; ++i) {
        for (k = 0; k < 3; ++k) {
            normal_index = model->triangles[i].points[k].normal_index;
            normal_x = model->normals[normal_index].x;
            normal_y = model->normals[normal_index].y;
            normal_z = model->normals[normal_index].z;
            glNormal3d(normal_x, normal_y, normal_z);
            vertex_index = model->triangles[i].points[k].vertex_index;
            x = model->vertices[vertex_index].x;
            y = model->vertices[vertex_index].y;
            z = model->vertices[vertex_index].z;
            switch (k) {
                case 0:
                    glTexCoord2f(0, 0);
                    break;
                case 1:
                    glTexCoord2f(0.1 * z, 0);
                    break;
                case 2:
                    glTexCoord2f(0, 0.01);
                    break;
            }
            glVertex3d(x, y, z);

        }
    }

    glEnd();
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

void draw_normals(const struct Model *model, double length) {
    int i;
    double x1, y1, z1, x2, y2, z2;

    glColor3f(0, 0, 1);

    glBegin(GL_LINES);

    for (i = 0; i < model->n_vertices; ++i) {
        x1 = model->vertices[i].x;
        y1 = model->vertices[i].y;
        z1 = model->vertices[i].z;
        x2 = x1 + model->normals[i].x * length;
        y2 = y1 + model->normals[i].y * length;
        z2 = z1 + model->normals[i].z * length;
        glVertex3d(x1, y1, z1);
        glVertex3d(x2, y2, z2);
    }
    glEnd();
}

void draw_bounding_box(const Model *model) {
    glLineWidth(1);

    Vertex half_diag;
    half_diag.x = (model->box.maxVertex.x - model->box.minVertex.x) / 2;
    half_diag.y = (model->box.maxVertex.y - model->box.minVertex.y) / 2;
    half_diag.z = (model->box.maxVertex.z - model->box.minVertex.z) / 2;

    glBegin(GL_LINES);
    glVertex3d(model->box.minVertex.x, model->box.minVertex.y, model->box.minVertex.z);
    glVertex3d(model->box.maxVertex.x, model->box.maxVertex.y, model->box.maxVertex.z);
    glEnd();

    /* float m[16];
     glGetFloatv(GL_MODELVIEW_MATRIX, &m);

     float proj [16];
     glGetFloatv(GL_PROJECTION_MATRIX, &proj);

     int view[4];
     glGetIntegerv(GL_VIEWPORT, &view);*/


}


void draw_model(const Model *model) {
    //draw_triangles(model);
    draw_quads(model);
    draw_bounding_box(model);

}

void draw_skybox_bottom(Entity skybox) {
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
            glTexCoord2d(u, 1.0 - v1);
            glVertex3d(x1, y1, -z1);
            glTexCoord2d(u, 1.0 - v2);
            glVertex3d(x2, y2, -z2);
        }
    }

    glEnd();
    glPopMatrix();
}


void draw_skybox_top(Entity skybox) {
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
            glTexCoord2d(u, 1.0 - v1);
            glVertex3d(x1, y1, z1);
            glTexCoord2d(u, 1.0 - v2);
            glVertex3d(x2, y2, z2);
        }
    }

    glEnd();
    glPopMatrix();
}

bool is_point_inside_sphere(Move mov, Move mov2, Model planet) {


}


void draw_environment(World world, Rotate *rotate, Move move) {

    glEnable(GL_TEXTURE_2D);

    //Draw the bottom skybox.
    draw_skybox_bottom(world.skybox);

    //Draw the top skybox.
    draw_skybox_top(world.skybox);

    //Draw the sun.
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world.sun.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world.sun.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(rotate->sun_rotation, 1, 1, 1);
    draw_model(&world.sun.model);
    glPopMatrix();

    //Draw the planet1.
    glPushMatrix();
    glTranslatef(move.planet1.x, move.planet1.y, move.planet1.z);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world.planet1.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world.planet1.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(rotate->planet1_rotation, 0, 0, 1);
    draw_model(&world.planet1.model);

    glPopMatrix();


    //Draw the planet2.
    glPushMatrix();

    glTranslatef(move.planet1.x + 1000, move.planet1.y + 1000, move.planet1.z - 100);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world.planet2.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world.planet2.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(180, 0, 0, 1);
    glRotatef(rotate->planet2_rotation, 0, 0, 1);
    draw_model(&world.planet2.model);

    glPopMatrix();


    //Draw the planet3.
    glPushMatrix();
    glTranslatef(move.planet3.x, move.planet3.y, move.planet3.z);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world.planet3.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world.planet3.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(rotate->planet3_rotation, 0, 0, -1);
    draw_model(&world.planet3.model);
    glPopMatrix();

    //Draw the planet4.
    glPushMatrix();
    glTranslatef(move.planet4.x, move.planet4.y, move.planet4.z);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world.planet4.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world.planet4.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(rotate->planet4_rotation, 0, 0, -1);
    draw_model(&world.planet4.model);
    glPopMatrix();

    //Draw the satellite.
    glPushMatrix();
    glTranslatef(move.satellite.x, move.satellite.y, move.satellite.z);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world.satellite.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world.satellite.texture);

    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(90, 1, 0, 0);
    glRotatef(270, 0, 1, 0);
    glRotatef(rotate->satellite_rotation, 0, 0, 1);
    draw_model(&world.satellite.model);
    glPopMatrix();
}