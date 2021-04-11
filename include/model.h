#ifndef MODEL_H_
#define MODEL_H_

#include "basic.h"
#include <stdio.h>
#include <stdbool.h>
#include "SOIL/SOIL.h"
#include <math.h>

#define TRUE 1
#define FALSE 0
#define INVALID_VERTEX_INDEX 0

struct Triangle {
    struct FacePoint points[3];
};

struct Quad {
    struct FacePoint points[4];
};

struct BBox {
    Vertex minVertex;
    Vertex maxVertex;
    double diagonal_length;
};

typedef struct Model {
    int n_vertices;
    int n_texture_vertices;
    int n_normals;
    int n_triangles;
    int n_quads;
    struct Vertex *vertices;
    struct TextureVertex *texture_vertices;
    struct Vertex *normals;
    struct Triangle *triangles;
    struct Quad *quads;
    struct BBox box;
} Model;

typedef struct Entity {
    Model model;
    int texture;
    int texture2;
    float material_ambient[4];
} Entity;

typedef union World {
    struct {
        Entity jupiter;
        Entity jupiter_moon;
        Entity venus;
        Entity saturnus;
        Entity sun;
        Entity satellite;
        Entity skybox;
    };
    Entity World[7];
} World;

typedef struct Camera {
    struct Vertex position;
    struct Vertex pose;
    struct Vertex prev_position;
} Camera;

typedef struct Action {
    int move_forward;
    int move_backward;
    int step_left;
    int step_right;
    int move_up;
    int move_down;
    int rotate_planets_in_galaxy;
    int move_jupiter_plus_moon_in_galaxy;
    int move_venus_in_galaxy;
    int move_saturnus_in_galaxy;
    int call_satellite;
    int increase_light;
    int decrease_light;
} Action;

extern Camera camera;
extern Action action;


typedef union Position {
    struct {
        double x;
        double y;
        double z;
    };
    double Position[3];
} Position;

typedef struct Rotate {
    double planet1_rotation;
    double planet2_rotation;
    double planet3_rotation;
    double planet4_rotation;
    double satellite_rotation;
    double sun_rotation;
} Rotate;

typedef union Move {
    struct {
        Position jupiter;
        Position jupiter_moon;
        Position venus;
        Position saturnus;
        Position sun;
        Position satellite;
    };
    Position Move[6];
} Move;

extern Rotate rotate;
extern Move move;


typedef struct Data {
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
    int mouse_x;
    int mouse_y;
    int previous_time;
    int help, help_on;
    float speed;
    float angle;
    double e_time;
} Data;

extern GLfloat light_ambient[];
extern World world;


extern Data data;

//Count the tokens in the text.
int count_tokens(const char *text);


//Calculate the length of the token.
int calc_token_length(const char *text, int start_index);


//Create the copy of the token from the given string.
char *copy_token(const char *text, int offset, int length);


//Insert token to the token structure.
void insert_token(const char *token, struct TokenArray *token_array);


//Extract tokens from text.
void extract_tokens(const char *text, struct TokenArray *token_array);


//Release the allocate memory of the token array.
void free_tokens(struct TokenArray *token_array);


//Load OBJ model from file.
int load_model(const char *filename, struct Model *model);


//Print the information about the loaded model.
void print_model_info(const struct Model *model);


//Release the allocated memory of the model.
void free_model(struct Model *model);


//Count the elements in the model and set counts in the structure.
void count_elements(FILE *file, struct Model *model);


//Read the elements of the model and fill the structure with values.
void read_elements(FILE *file, struct Model *model);


//Initializes model counters to zero.
void init_model_counters(struct Model *model);


//Clear the comment from the end of the line.
void clear_comment(char *line);


//Determine the type of the line and increment the appropriate counter.
void count_element_in_line(const char *line, struct Model *model);


//Read the given data from the line.
void read_element_from_line(const char *line, struct Model *model);


//Allocate memory for the model.
void create_arrays(struct Model *model);


//Read vertex data.
void read_vertex(const struct TokenArray *token_array, struct Vertex *vertex);


//Read texture vertex data.
void read_texture_vertex(const struct TokenArray *token_array, struct TextureVertex *texture_vertex);


//Read normal vector data.
void read_normal(const struct TokenArray *token_array, struct Vertex *normal);


//Read triangle data.
void read_triangle(const struct TokenArray *token_array, struct Triangle *triangle);


//Read quad data.
void read_quad(const struct TokenArray *token_array, struct Quad *quad);


//Read face point data.
void read_face_point(const char *text, struct FacePoint *face_point);


//Count the delimiters in the face token.
int count_face_delimiters(const char *text);


//Read the next index from the string.
int read_next_index(const char *text, int *length);


//Check that the next number is a number digit.
int is_digit(char c);


//Check that the indices in the triangle are valid.
int is_valid_triangle(const struct Triangle *triangle, const struct Model *model);

//Check that the indices in the quad are valid.
int is_valid_quad(const struct Quad *quad, const struct Model *model);

//Print the bounding box of the model.
void calc_bounding_box(struct Model *model);

//Scale the loaded model.
void scale_model(struct Model *model, double sx, double sy, double sz);

//Load the entities object and texture.
void init_entities(World *world);

GLuint load_texture(const char *filename);

double vector_length(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z);

Vertex vector_from_two_vertex(double a_x, double a_y, double a_z, double b_x, double b_y, double b_z);

#endif
