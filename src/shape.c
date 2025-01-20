#include "shape.h"

static void Shape_draw(Shape const * const me);
static uint32_t Shape_area(Shape const * const me);

void Shape_ctor(Shape* const me, int16_t x0, int16_t y0) {
    static const struct ShapeVtable vtable = {
        &Shape_draw,
        &Shape_area,
    };
    me -> vptr = &vtable;
    me -> x = x0;
    me -> y = y0;
}
void Shape_move_by(Shape* const me, int16_t dx, int16_t dy) {
    me -> x += dx;
    me -> y += dy;
}

uint16_t Shape_distance_from(Shape const* const me, Shape const* other) {
    int16_t dx = me -> x - other -> x;
    int16_t dy = me -> y - other -> y;

    if (dx < 0 ) {
        dx = -dx;
    }
    if (dy < 0 ) {
        dy = -dy;
    }
    return dx + dy;
}

void Shape_draw(Shape const * const me) {
    (void)me;
}
uint32_t Shape_area(Shape const * const me) {
    (void)me;
    return 0U;
}

void drawGraph(Shape const * graph[]) {
    for (uint8_t i = 0; graph[i] != (Shape *)0; i++) {
        Shape_draw_vcall(graph[i]); /* polymorphism */
    }
}


