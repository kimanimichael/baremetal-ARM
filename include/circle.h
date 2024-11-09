#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

class Circle : public Shape {
public:
    Circle(
        int16_t  x0, int16_t y0,
        uint16_t r0);

private:
    uint16_t radius;

public:
    void draw(int16_t test) override;

    uint32_t area() const override;
};


#endif
