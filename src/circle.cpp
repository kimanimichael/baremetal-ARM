#include "circle.h"

Circle::Circle(int16_t  x0, int16_t y0,
               uint16_t r0)
    : Shape(x0, y0),
      radius(r0) {
}

void Circle::draw(int16_t test) {
    x += test;
    // drawEllipse(x, y, radius, radius);
    //drawHorLine(x, y, width);
    //drawVerLine(x + width,
    //            y, height);
    //drawHorLine(x + width,
    //            y + height, width);
    //drawVerLine(x, y, height);
}

uint32_t Circle::area() const {
    return 3U * static_cast<uint32_t>(radius) * static_cast<uint32_t>(radius);
}
