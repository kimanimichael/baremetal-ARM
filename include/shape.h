#ifndef SHAPE_H
#define SHAPE_H

#include "stdint.h"

/* Shape's attributes */
typedef struct {
    struct ShapeVtable const *vptr; /* virtual pointer */
    int16_t x;
    int16_t y;
} Shape;

struct ShapeVtable {
    void (*draw)(Shape const * const me);
    uint32_t (*area)(Shape const * const me);
};

/* Shape's operations */
void Shape_ctor(Shape * const me, int16_t x0, int16_t y0);
void Shape_move_by(Shape * const me, int16_t dx, int16_t dy);
uint16_t Shape_distance_from(Shape const * const me,
                             Shape const * other);

/* virtual calls - late binding */
static inline void Shape_draw_vcall(Shape const * const me) {
    (*me->vptr->draw)(me);
}

static inline uint32_t Shape_area_vcall(Shape const * const me) {
    return  (*me->vptr->area)(me);
}

void drawGraph(Shape const * graph[]);

#endif
