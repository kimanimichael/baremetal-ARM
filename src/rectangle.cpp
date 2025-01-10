#include "rectangle.h"

Rectangle::Rectangle(int16_t  x0, int16_t  y0,
                     uint16_t w0, uint16_t h0)
    : Shape(x0, y0),
      width(w0),
      height(h0) {
}

void Rectangle::draw() {
    //drawHorLine(x, y, width);
    //drawVerLine(x + width,
    //            y, height);
    //drawHorLine(x + width,
    //            y + height, width);
    //drawVerLine(x, y, height);
}

uint32_t Rectangle::area() const {
    return static_cast<uint32_t>(width) * static_cast<uint32_t>(height);
}
