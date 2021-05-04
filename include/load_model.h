#ifndef LOAD_H
#define LOAD_H

#include "model.h"
#include <stdio.h>

#define INVALID_VERTEX_INDEX 0

struct TokenArray {
    char **tokens;
    int n_tokens;
};

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

//Load the entities object and texture.
void init_entities(World *world);

GLuint load_texture(const char *filename);

#endif
