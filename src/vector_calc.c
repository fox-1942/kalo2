//
// Created by Fox-1942 on 02/04/2021.
//

#include <stdio.h>
#include "vector_calc.h"

double vector_length(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z) {
    double diagonal_x = max_x - min_x;
    double diagonal_y = max_y - min_y;
    double diagonal_z = max_z - min_z;

   /* printf("hid: %f %f %f ",diagonal_x, diagonal_y,diagonal_z);
    printf("Pow-ok: %f %f %f ",pow(diagonal_x, 2), pow(diagonal_y, 2),pow(diagonal_z, 2));
    printf("Result: %f ",sqrt(pow(diagonal_x, 2) + pow(diagonal_y, 2) + pow(diagonal_z, 2)));*/

    return sqrt(pow(diagonal_x, 2) + pow(diagonal_y, 2) + pow(diagonal_z, 2));
}
