#ifndef SHAPE_H
#define SHAPE_H

#include "stdint.h"

// /* Shape's attributes */
// typedef struct {
//     int16_t x;
//     int16_t y;
// } Shape;

// /* Shape's operations */
// void Shape_ctor(Shape * const me, int16_t x0, int16_t y0);
// void Shape_move_by(Shape * const me, int16_t dx, int16_t dy);
// uint16_t Shape_distance_from(Shape const * const me,
//                              Shape const * other);

class Shape {
public:
    /**
    * @param x0 initialized x-coordinate
    * @param y0 initialized y-coordinate
    */
    Shape(int16_t x0, int16_t y0);
protected:
    /**
    * x-coordinate of shape
    */
    int16_t x;
    /**
    * y-coordinate of shape
    */
    int16_t y;
public:
    /**
    * @brief Move shape
    * @param dx distance to move shape by in the x direction
    * @param dy distance to move shape by in the y direction
    */
    void move_by(int16_t dx, int16_t dy);
    /**
    * @brief Get distance from another shape instance
    * @param other shape to get distance from
    * @return distance from other shape
    */
    uint16_t distance_from(Shape const * other)const;
    /**
    * @brief draw shape interface
    * @param test test parameter
    */
    virtual void draw();
    /**
    * @brief calculate area of shape
    * @return shape area
    */
    virtual uint32_t area()const;

};

void drawGraph(Shape *graph[]);

#endif
