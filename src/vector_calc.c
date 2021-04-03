//
// Created by Fox-1942 on 02/04/2021.
//

#include "vector_calc.h"

double vector_length(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z) {
    double diagonal_x = max_x - min_x;
    double diagonal_y = max_y - min_y;
    double diagonal_z = max_z - min_z;
    return sqrt(pow(diagonal_x, 2) + pow(diagonal_y, 2) + pow(diagonal_z, 2));
}
