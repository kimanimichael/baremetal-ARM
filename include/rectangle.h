#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

class Rectangle : public Shape {
public:
    Rectangle(
        int16_t  x0, int16_t  y0,
        uint16_t w0, uint16_t h0);

private:
    uint16_t width;
    uint16_t height;

public:
    void draw(int16_t test) override;

    uint32_t area()const override;
};


#endif
