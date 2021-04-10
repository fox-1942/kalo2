//
// Created by Fox-1942 on 10/04/2021.
//

#ifndef KALO_BASIC_H
#define KALO_BASIC_H

#include <GL/glut.h>
#define M_PI 3.14159265358979323846

typedef GLubyte Pixel;
struct TokenArray {
    char **tokens;
    int n_tokens;
};

typedef struct Vertex {
    double x;
    double y;
    double z;

} Vertex;

struct TextureVertex {
    double u;
    double v;
};

struct FacePoint {
    int vertex_index;
    int texture_index;
    int normal_index;
};

double degree_to_radian(double degree);

#endif //KALO_BASIC_H
