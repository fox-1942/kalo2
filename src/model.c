#include "model.h"

#define LINE_BUFFER_SIZE 1024

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


int count_tokens(const char *text) {
    int i = 0;
    int is_token = FALSE;
    int count = 0;

    while (text[i] != 0) {
        if (is_token == FALSE && text[i] != ' ') {
            ++count;
            is_token = TRUE;
        } else if (is_token == TRUE && text[i] == ' ') {
            is_token = FALSE;
        }
        ++i;
    }

    return count;
}


void extract_tokens(const char *text, struct TokenArray *token_array) {
    int n_tokens, token_length;
    char *token;
    int i;

    n_tokens = count_tokens(text);

    token_array->tokens = (char **) malloc(n_tokens * sizeof(char *));
    token_array->n_tokens = 0;

    i = 0;
    while (text[i] != 0) {
        if (text[i] != ' ') {
            token_length = calc_token_length(text, i);
            token = copy_token(text, i, token_length);
            insert_token(token, token_array);
            i += token_length;
        } else {
            ++i;
        }
    }
}


char *copy_token(const char *text, int offset, int length) {
    char *token;
    int i;

    token = (char *) malloc((length + 1) * sizeof(char));
    for (i = 0; i < length; ++i) {
        token[i] = text[offset + i];
    }
    token[i] = 0;

    return token;
}


void insert_token(const char *token, struct TokenArray *token_array) {
    token_array->tokens[token_array->n_tokens] = (char *) token;
    ++token_array->n_tokens;
}


int calc_token_length(const char *text, int start_index) {
    int end_index, length;

    end_index = start_index;
    while (text[end_index] != 0 && text[end_index] != ' ') {
        ++end_index;
    }
    length = end_index - start_index;

    return length;
}


void free_tokens(struct TokenArray *token_array) {
    int i;

    for (i = 0; i < token_array->n_tokens; ++i) {
        free(token_array->tokens[i]);
    }
    free(token_array->tokens);
}


void print_model_info(const struct Model *model) {
    printf("Vertices: %d\n", model->n_vertices);
    printf("Texture vertices: %d\n", model->n_texture_vertices);
    printf("Normals: %d\n", model->n_normals);
    printf("Triangles: %d\n", model->n_triangles);
    printf("Quads: %d\n", model->n_quads);
}


void free_model(struct Model *model) {
    free(model->vertices);
    free(model->texture_vertices);
    free(model->normals);
    free(model->triangles);
    free(model->quads);
}

void init_model_counters(Model *model) {
    model->n_vertices = 0;
    model->n_texture_vertices = 0;
    model->n_normals = 0;
    model->n_triangles = 0;
    model->n_quads = 0;
}

void count_elements(FILE *file, struct Model *model) {
    char line[LINE_BUFFER_SIZE];

    init_model_counters(model);
    while (fgets(line, LINE_BUFFER_SIZE, file) != NULL) {
        clear_comment(line);
        count_element_in_line(line, model);
    }
}


void read_elements(FILE *file, struct Model *model) {
    char line[LINE_BUFFER_SIZE];

    init_model_counters(model);
    model->n_vertices = 1;
    model->n_texture_vertices = 1;
    model->n_normals = 1;

    fseek(file, 0, SEEK_SET);
    while (fgets(line, LINE_BUFFER_SIZE, file) != NULL) {
        clear_comment(line);
        read_element_from_line(line, model);
    }
}

void clear_comment(char *line) {
    int i = 0;
    while (line[i] != 0 && line[i] != '#' && line[i] != 0x0D && line[i] != 0x0A) {
        ++i;
    }
    while (line[i] != 0) {
        line[i] = ' ';
        ++i;
    }
}


void count_element_in_line(const char *line, Model *model) {
    struct TokenArray token_array;
    char *first_token;

    extract_tokens(line, &token_array);

    if (token_array.n_tokens > 0) {
        first_token = token_array.tokens[0];
        if (strcmp(first_token, "v") == 0) {
            ++model->n_vertices;
        } else if (strcmp(first_token, "vt") == 0) {
            ++model->n_texture_vertices;
        } else if (strcmp(first_token, "vn") == 0) {
            ++model->n_normals;
        } else if (strcmp(first_token, "f") == 0) {
            if (token_array.n_tokens == 1 + 3) {
                ++model->n_triangles;
            } else if (token_array.n_tokens == 1 + 4) {
                ++model->n_quads;
            } else {
                //printf("WARN: Invalid number of face elements! %d\n", token_array.n_tokens);
            }
        }
    }

    free_tokens(&token_array);
}


void read_element_from_line(const char *line, Model *model) {
    struct TokenArray token_array;
    char *first_token;
    struct Triangle *triangle;
    struct Quad *quad;

    extract_tokens(line, &token_array);

    if (token_array.n_tokens > 0) {
        first_token = token_array.tokens[0];
        if (strcmp(first_token, "v") == 0) {
            read_vertex(&token_array, &(model->vertices[model->n_vertices]));
            ++model->n_vertices;
        } else if (strcmp(first_token, "vt") == 0) {
            read_texture_vertex(&token_array, &(model->texture_vertices[model->n_texture_vertices]));
            ++model->n_texture_vertices;
        } else if (strcmp(first_token, "vn") == 0) {
            read_normal(&token_array, &(model->normals[model->n_normals]));
            ++model->n_normals;
        } else if (strcmp(first_token, "f") == 0) {
            if (token_array.n_tokens == 1 + 3) {
                triangle = &(model->triangles[model->n_triangles]);
                read_triangle(&token_array, triangle);
                if (is_valid_triangle(triangle, model) == FALSE) {
                    printf("line: '%s'\n", line);
                }
                ++model->n_triangles;
            } else if (token_array.n_tokens == 1 + 4) {
                quad = &(model->quads[model->n_quads]);
                read_quad(&token_array, quad);
                if (is_valid_quad(quad, model) == FALSE) {
                    printf("line: '%s'\n", line);
                }
                ++model->n_quads;
            }
        }
    }

    free_tokens(&token_array);
}


void create_arrays(struct Model *model) {
    model->vertices = (struct Vertex *) malloc((model->n_vertices + 1) * sizeof(struct Vertex));
    model->texture_vertices = (struct TextureVertex *) malloc(
            (model->n_texture_vertices + 1) * sizeof(struct TextureVertex));
    model->normals = (struct Vertex *) malloc((model->n_normals + 1) * sizeof(struct Vertex));
    model->triangles = (struct Triangle *) malloc(model->n_triangles * sizeof(struct Triangle));
    model->quads = (struct Quad *) malloc(model->n_quads * sizeof(struct Quad));
}


void read_vertex(const struct TokenArray *token_array, struct Vertex *vertex) {
    vertex->x = atof(token_array->tokens[1]);
    vertex->y = atof(token_array->tokens[2]);
    vertex->z = atof(token_array->tokens[3]);
}


void read_texture_vertex(const struct TokenArray *token_array, struct TextureVertex *texture_vertex) {
    texture_vertex->u = atof(token_array->tokens[1]);
    texture_vertex->v = atof(token_array->tokens[2]);
}


void read_normal(const struct TokenArray *token_array, struct Vertex *normal) {
    normal->x = atof(token_array->tokens[1]);
    normal->y = atof(token_array->tokens[2]);
    normal->z = atof(token_array->tokens[3]);
}


void read_triangle(const struct TokenArray *token_array, struct Triangle *triangle) {
    int i;

    for (i = 0; i < 3; ++i) {
        read_face_point(token_array->tokens[i + 1], &triangle->points[i]);
    }
}


void read_quad(const struct TokenArray *token_array, struct Quad *quad) {
    int i;

    for (i = 0; i < 4; ++i) {
        read_face_point(token_array->tokens[i + 1], &quad->points[i]);
    }
}


void read_face_point(const char *text, struct FacePoint *face_point) {
    int delimiter_count;
    const char *token;
    int length;

    token = text;
    delimiter_count = count_face_delimiters(text);

    if (delimiter_count == 0) {
        face_point->vertex_index = read_next_index(token, &length);
        face_point->texture_index = INVALID_VERTEX_INDEX;
        face_point->normal_index = INVALID_VERTEX_INDEX;
    } else if (delimiter_count == 1) {
        face_point->vertex_index = read_next_index(token, &length);
        token += length;
        face_point->texture_index = read_next_index(token, &length);
        face_point->normal_index = INVALID_VERTEX_INDEX;
    } else if (delimiter_count == 2) {
        face_point->vertex_index = read_next_index(token, &length);
        token += length;
        face_point->texture_index = read_next_index(token, &length);
        token += length;
        face_point->normal_index = read_next_index(token, &length);
    } else {
        printf("ERROR: Invalid face token! '%s'", text);
    }
}


int count_face_delimiters(const char *text) {
    int count, i;

    count = 0;
    i = 0;
    while (text[i] != 0) {
        if (text[i] == '/') {
            ++count;
        }
        ++i;
    }

    return count;
}


int read_next_index(const char *text, int *length) {
    int i, j, index;
    char buffer[32];

    i = 0;
    while (text[i] != 0 && is_digit(text[i]) == FALSE) {
        ++i;
    }

    if (text[i] == 0) {
        return INVALID_VERTEX_INDEX;
    }

    j = 0;
    while (text[i] != 0 && is_digit(text[i]) == TRUE) {
        buffer[j] = text[i];
        ++i;
        ++j;
    }
    buffer[j] = 0;

    index = atoi(buffer);
    *length = i;

    return index;
}


int is_digit(char c) {
    if (c >= '0' && c <= '9') {
        return TRUE;
    }
    return FALSE;
}


int is_valid_triangle(const struct Triangle *triangle, const struct Model *model) {
    int k;

    for (k = 0; k < 3; ++k) {
        if (triangle->points[k].vertex_index >= model->n_vertices) {
            printf("ERROR: Invalid vertex index in a triangle!\n");
            return FALSE;
        }
        if (triangle->points[k].texture_index >= model->n_texture_vertices) {
            printf("ERROR: Invalid texture vertex index in a triangle!\n");
            return FALSE;
        }
        if (triangle->points[k].normal_index >= model->n_normals) {
            printf("ERROR: Invalid normal index in a triangle!\n");
            return FALSE;
        }
    }
    return TRUE;
}


int is_valid_quad(const struct Quad *quad, const struct Model *model) {
    int k;
    int vertex_index, texture_index, normal_index;

    for (k = 0; k < 4; ++k) {
        vertex_index = quad->points[k].vertex_index;
        texture_index = quad->points[k].texture_index;
        normal_index = quad->points[k].normal_index;
        if (vertex_index < 0 || vertex_index >= model->n_vertices) {
            printf("ERROR: Invalid vertex index in a quad!\n");
            return FALSE;
        }
        if (texture_index < 0 || texture_index >= model->n_texture_vertices) {
            printf("ERROR: Invalid texture vertex index in a quad!\n");
            return FALSE;
        }
        if (normal_index < 0 || normal_index >= model->n_normals) {
            printf("ERROR: Invalid normal index in a quad!");
            return FALSE;
        }
    }
    return TRUE;
}


void calc_bounding_box(struct Model *model) {
    int i;
    double x, y, z;
    double min_x, max_x, min_y, max_y, min_z, max_z;

    if (model->n_vertices == 0) { return; }

    min_x = model->vertices[1].x;
    max_x = model->vertices[1].x;
    min_y = model->vertices[1].y;
    max_y = model->vertices[1].y;
    min_z = model->vertices[1].z;
    max_z = model->vertices[1].z;

    for (i = 1; i < model->n_vertices; ++i) {
        x = model->vertices[i].x;
        y = model->vertices[i].y;
        z = model->vertices[i].z;
        if (x < min_x) {
            min_x = x;
        } else if (x > max_x) {
            max_x = x;
        }
        if (y < min_y) {
            min_y = y;
        } else if (y > max_y) {
            max_y = y;
        }
        if (z < min_z) {
            min_z = z;
        } else if (z > max_z) {
            max_z = z;
        }
    }
    /*printf("Bounding box:\n");
    printf("x in [%lf, %lf]\n", min_x, max_x);
    printf("y in [%lf, %lf]\n", min_y, max_y);
    printf("z in [%lf, %lf]\n", min_z, max_z);*/

    // setting the coordinates of the bounding box for the specific model
    model->box.minVertex.x = min_x;
    model->box.minVertex.y = min_y;
    model->box.minVertex.z = min_z;
    model->box.maxVertex.x = max_x;
    model->box.maxVertex.y = max_y;
    model->box.maxVertex.z = max_z;

    model->box.diagonal_length = vector_length(min_x, min_y, min_z, max_x, max_y, max_z);
    printf("Bbox diagonal length: %f \n", model->box.diagonal_length);

}

void scale_model(struct Model *model, double sx, double sy, double sz) {
    int i;

    for (i = 0; i < model->n_vertices; ++i) {
        model->vertices[i].x *= sx;
        model->vertices[i].y *= sy;
        model->vertices[i].z *= sz;
    }
}


int load_model(const char *filename, Model *model) {
    FILE *obj_file = fopen(filename, "r");
    printf("Load model '%s' ...\n", filename);
    if (obj_file == NULL) {
        printf("ERROR: Unable to open '%s' file!\n", filename);
        return FALSE;
    }
    printf("Count ..\n");
    count_elements(obj_file, model);
    printf("Create ..\n");
    create_arrays(model);
    printf("Read ..\n");
    read_elements(obj_file, model);
    fclose(obj_file);

    return TRUE;
}

void init_entities(World *world) {
    //Load the planet1 object and texture.
    load_model("..\\objects\\geoid.obj", &world->jupiter.model);
    world->jupiter.texture = load_texture("..\\textures\\planet2.png");
    scale_model(&world->jupiter.model, 0.4, 0.4, 0.4);

    //Load the planet2 and texture.
    load_model("..\\objects\\geoid.obj", &world->jupiter_moon.model);
    world->jupiter_moon.texture = load_texture("..\\textures\\planet3.png");
    scale_model(&world->jupiter_moon.model, 0.2, 0.2, 0.2);

    //Load the planet3 and texture.
    load_model("..\\objects\\geoid.obj", &world->venus.model);
    world->venus.texture = load_texture("..\\textures\\planet1.png");
    scale_model(&world->venus.model, 0.5, 0.5, 0.5);

    //Load the planet4 and texture.
    load_model("..\\objects\\saturn.obj", &world->saturnus.model);
    world->saturnus.texture = load_texture("..\\textures\\planet4.jpg");
    scale_model(&world->saturnus.model, 0.2, 0.2, 0.2);

    //Load the sun object and texture.
    load_model("..\\objects\\geoid.obj", &world->sun.model);
    world->sun.texture = load_texture("..\\textures\\sun.png");
    scale_model(&world->sun.model, 2, 2, 2);

    //Load the satellite object and texture.
    load_model("..\\objects\\satellite.obj", &world->satellite.model);
    world->satellite.texture = load_texture("..\\textures\\satellite.jpg");
    world->satellite.texture2 = load_texture("..\\textures\\satellite2.jpg");

    scale_model(&world->satellite.model, 12, 12, 12);

    //Load the skybox texture.
    world->skybox.texture = load_texture("..\\textures\\sky.png");

    //Storing bounding-box coordinates in each model
    calc_bounding_box(&world->jupiter.model);
    calc_bounding_box(&world->jupiter_moon.model);
    calc_bounding_box(&world->venus.model);
    calc_bounding_box(&world->saturnus.model);
    calc_bounding_box(&world->sun.model);
    world->sun.model.box.diagonal_length-=10; // small optimalization for better demonstration.
}

double vector_length(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z) {
    double diagonal_x = max_x - min_x;
    double diagonal_y = max_y - min_y;
    double diagonal_z = max_z - min_z;

    return sqrt(pow(diagonal_x, 2) + pow(diagonal_y, 2) + pow(diagonal_z, 2));
}

Vertex vector_from_two_vertex(double a_x, double a_y, double a_z, double b_x, double b_y, double b_z) {
    return (Vertex) {.x =  a_x - b_x, .y = a_y - b_y, .z = a_z - b_z};
}