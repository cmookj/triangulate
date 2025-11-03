#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include <array>
#include <vector>

//// Point struct and related functions
struct Point {
    double x = 0.;
    double y = 0.;

    void
    operator+= (const Point& p) {
        this->x += p.x;
        this->y += p.y;
    }
};

Point
operator+ (const Point& a, const Point& b);

bool
operator== (const Point& a, const Point& b);

bool
close_enough (const Point& a, const Point& b, const double threshold = 1e-12);

//// Struct: Polar (coordinates)
struct Polar {
    double length;
    double angle;  // In radians
};

//// Type Aliases
using TriangleSpec = std::array<std::size_t, 3>;
using Triangles    = std::vector<TriangleSpec>;
using Points       = std::vector<Point>;

#endif
