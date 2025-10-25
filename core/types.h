#ifndef __TYPES_H__
#define __TYPES_H__

#include <array>
#include <vector>

//// Point struct and related functions
struct Point {
    double x = 0.;
    double y = 0.;
};

//// Struct: Polar (coordinates)
struct Polar {
    double length;
    double angle;  // In radians
};

//// Type Aliases
using TriangleSpec = std::array<size_t, 3>;
using Triangles    = std::vector<TriangleSpec>;
using Points       = std::vector<Point>;

#endif
