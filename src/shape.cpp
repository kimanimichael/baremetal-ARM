#include "shape.h"

Shape::Shape(int16_t x0, int16_t y0)
    : x(x0), y(y0)
{
}

void Shape::move_by(int16_t dx, int16_t dy) {
    x += dx;
    y += dy;
}

uint16_t Shape::distance_from(Shape const* other)const {
    int16_t dx = x - other -> x;
    int16_t dy = y - other -> y;

    if (dx < 0 ) {
        dx = -dx;
    }
    if (dy < 0 ) {
        dy = -dy;
    }
    return dx + dy;
}

void Shape::draw(int16_t test) {
    x += test;
    //drawHorLine(x, y, width);
    //drawVerLine(x + width,
    //            y, height);
    //drawHorLine(x + width,
    //            y + height, width);
    //drawVerLine(x, y, height);
}

uint32_t Shape::area() const {
    return 0U;
}



