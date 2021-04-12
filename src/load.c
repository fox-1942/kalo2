#include "load.h"

#include <stdlib.h>
#include <basic_primitives.h>
#include <SOIL/SOIL.h>

#define LINE_BUFFER_SIZE 1024

void print_model_info(const Model* model)
{
    printf("Vertices: %d---------------------------------------------\n", model->n_vertices);
    printf("Texture vertices: %d\n", model->n_texture_vertices);
    printf("Normals: %d\n", model->n_normals);
    printf("Triangles: %d--------------------------------------------\n", model->n_triangles);
}

int load_model(const char* filename, Model* model)
{
    FILE* obj_file;
    int success;

    obj_file = fopen(filename, "r");
    printf("Load model '%s' ...\n", filename);
    if (obj_file == NULL) {
        printf("ERROR: Unable to open '%s' file!\n", filename);
        return FALSE;
    }
    printf("Count the elements ...\n");
    count_elements(model, obj_file);
    printf("Allocate memory for model ...\n");
    allocate_model(model);
    printf("Read model data ...\n");
    fseek(obj_file, 0, SEEK_SET);
    success = read_elements(model, obj_file);
    if (success == FALSE) {
        printf("ERROR: Unable to read the model data!\n");
        return FALSE;
    }

    print_model_info(model);
    return TRUE;
}

void count_elements(Model* model, FILE* file)
{
    char line[LINE_BUFFER_SIZE];

    init_model(model);
    while (fgets(line, LINE_BUFFER_SIZE, file) != NULL) {
        switch (calc_element_type(line)) {
        case NONE:
            break;
        case VERTEX:
            ++model->n_vertices;
            break;
        case TEXTURE_VERTEX:
            ++model->n_texture_vertices;
            break;
        case NORMAL:
            ++model->n_normals;
            break;
        case FACE:
            ++model->n_triangles;
            break;
        }
    }
}

int read_elements(Model* model, FILE* file)
{
    char line[LINE_BUFFER_SIZE];
    int success;

    allocate_model(model);

    model->n_vertices = 1;
    model->n_texture_vertices = 1;
    model->n_normals = 1;
    model->n_triangles = 0;
    model->n_quads = 0;

    fseek(file, 0, SEEK_SET);
    while (fgets(line, LINE_BUFFER_SIZE, file) != NULL) {
        switch (calc_element_type(line)) {
        case NONE:
            break;
        case VERTEX:
            success = read_vertex(&(model->vertices[model->n_vertices]), line);
            if (success == FALSE) {
                printf("Unable to read vertex data!\n");
                return FALSE;
            }
            ++model->n_vertices;
            break;
        case TEXTURE_VERTEX:
            success = read_texture_vertex(&(model->texture_vertices[model->n_texture_vertices]), line);
            if (success == FALSE) {
                printf("Unable to read texture vertex data!\n");
                return FALSE;
            }
            ++model->n_texture_vertices;
            break;
        case NORMAL:
            success = read_normal(&(model->normals[model->n_normals]), line);
            if (success == FALSE) {
                printf("Unable to read normal vector data!\n");
                return FALSE;
            }
            ++model->n_normals;
            break;
        case FACE:
            success = read_triangle(&(model->triangles[model->n_triangles]), line);
            if (success == FALSE) {
                printf("Unable to read triangle face data!\n");
                return FALSE;
            }
            ++model->n_triangles;
            break;
        }
    }
    return TRUE;
}

ElementType calc_element_type(const char* text)
{
    int i;

    i = 0;
    while (text[i] != 0) {
        if (text[i] == 'v') {
            if (text[i + 1] == 't') {
                return TEXTURE_VERTEX;
            }
            else if (text[i + 1] == 'n') {
                return NORMAL;
            }
            else {
                return VERTEX;
            }
        }
        else if (text[i] == 'f') {
            return FACE;
        }
        else if (text[i] != ' ' && text[i] != '\t') {
            return NONE;
        }
        ++i;
    }
    return NONE;
}

int read_vertex(Vertex* vertex, const char* text)
{
    int i;

    i = 0;
    while (text[i] != 0 && is_numeric(text[i]) == FALSE) {
        ++i;
    }
    if (text[i] != 0) {
        vertex->x = atof(&text[i]);
    }
    else {
        printf("The x value of vertex is missing!\n");
        return FALSE;
    }
    while (text[i] != 0 && text[i] != ' ') {
        ++i;
    }
    while (text[i] != 0 && is_numeric(text[i]) == FALSE) {
        ++i;
    }
    if (text[i] != 0) {
        vertex->y = atof(&text[i]);
    }
    else {
        printf("The y value of vertex is missing!\n");
        return FALSE;
    }
    while (text[i] != 0 && text[i] != ' ') {
        ++i;
    }
    while (text[i] != 0 && is_numeric(text[i]) == FALSE) {
        ++i;
    }
    if (text[i] != 0) {
        vertex->z = atof(&text[i]);
    }
    else {
        printf("The z value of vertex is missing!\n");
        return FALSE;
    }
    return TRUE;
}

int read_texture_vertex(TextureVertex* texture_vertex, const char* text)
{
    int i;

    i = 0;
    while (text[i] != 0 && is_numeric(text[i]) == FALSE) {
        ++i;
    }
    if (text[i] != 0) {
        texture_vertex->u = atof(&text[i]);
    }
    else {
        printf("The u value of texture vertex is missing!\n");
        return FALSE;
    }
    while (text[i] != 0 && text[i] != ' ') {
        ++i;
    }
    while (text[i] != 0 && is_numeric(text[i]) == FALSE) {
        ++i;
    }
    if (text[i] != 0) {
        texture_vertex->v = atof(&text[i]);
    }
    else {
        printf("The v value of texture vertex is missing!\n");
        return FALSE;
    }
    return TRUE;
}

int read_normal(Vertex* normal, const char* text)
{
    int i;

    i = 0;
    while (text[i] != 0 && is_numeric(text[i]) == FALSE) {
        ++i;
    }
    if (text[i] != 0) {
        normal->x = atof(&text[i]);
    }
    else {
        printf("The x value of normal vector is missing!\n");
        return FALSE;
    }
    while (text[i] != 0 && text[i] != ' ') {
        ++i;
    }
    while (text[i] != 0 && is_numeric(text[i]) == FALSE) {
        ++i;
    }
    if (text[i] != 0) {
        normal->y = atof(&text[i]);
    }
    else {
        printf("The y value of normal vector is missing!\n");
        return FALSE;
    }
    while (text[i] != 0 && text[i] != ' ') {
        ++i;
    }
    while (text[i] != 0 && is_numeric(text[i]) == FALSE) {
        ++i;
    }
    if (text[i] != 0) {
        normal->z = atof(&text[i]);
    }
    else {
        printf("The z value of normal vector is missing!\n");
        return FALSE;
    }
    return TRUE;
}

int read_triangle(Triangle* triangle, const char* text)
{
    int point_index;
    int i;

    i = 0;
    for (point_index = 0; point_index < 3; ++point_index) {
        while (text[i] != 0 && is_numeric(text[i]) == FALSE) {
            ++i;
        }
        if (text[i] != 0) {
            triangle->points[point_index].vertex_index = atoi(&text[i]);
        }
        else {
            printf("The vertex index of the %d. points is missing!\n", point_index + 1);
            return FALSE;
        }
        while (text[i] != 0 && text[i] != '/') {
            ++i;
        }
        ++i;
        if (text[i] != 0) {
            triangle->points[point_index].texture_index = atoi(&text[i]);
        }
        else {
            printf("The texture index of the %d. points is missing!\n", point_index + 1);
            return FALSE;
        }
        while (text[i] != 0 && text[i] != '/') {
            ++i;
        }
        ++i;
        if (text[i] != 0) {
            triangle->points[point_index].normal_index = atoi(&text[i]);
        }
        else {
            printf("The normal index of the %d. points is missing!\n", point_index + 1);
            return FALSE;
        }
        while (text[i] != 0 && text[i] != ' ') {
            ++i;
        }
    }
    return TRUE;
}

int is_numeric(char c)
{
    if ((c >= '0' && c <= '9') || c == '-' || c == '.') {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

GLuint load_texture(const char *filename) {

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GLuint texture_name;
    Pixel *image;
    glGenTextures(1, &texture_name);

    int width;
    int height;
    image = (Pixel *) SOIL_load_image(filename, &width, &height, 0,
                                      SOIL_LOAD_RGBA); // @suppress("Symbol is not resolved")

    glBindTexture(GL_TEXTURE_2D, texture_name);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (Pixel *) image);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    SOIL_free_image_data(image);
    return texture_name;
}