#ifndef SHAPE_H
#define SHAPE_H

#include "stdint.h"

/* Shape's attributes */
typedef struct {
    int16_t x;
    int16_t y;
} Shape;

/* Shape's operations */
void Shape_ctor(Shape * const me, int16_t x0, int16_t y0);
void Shape_move_by(Shape * const me, int16_t dx, int16_t dy);
uint16_t Shape_distance_from(Shape const * const me,
                             Shape const * other);

#endif
