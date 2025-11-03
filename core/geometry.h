//
// geometry.h
//
// Functions for simple geometry calculations
//
#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include "core/primitive.h"

//// NAMESPACE: geometry
namespace geometry {

// Distance between two points
double
distance (const Point& p, const Point& q);

// Angle of a line segment w.r.t. x-axis
double
angle (const Point& p, const Point& q);

// Calculates polar coordinate representation of a line segment
// defined by two points.
Polar
polar_coordinate (const Point& p, const Point& q);

// Constrain the angle of rotation between -180 andd 180
double
constrain_rotational_angle (const double q);

// Determine whether two line segments intersect or not.
// First line segment is from p to q.
// Second line segment is from r to s.
//
// The intersection is the solution of the following vector equation:
//   p + a(q - p) = r + b(s - r),
// where a anb b are scalars.
//
// In matrix form:
//   [q - p  r - s][a  b]T = [r - p]
// If the determinant of [q - p  r - s] is 0, there is no intersection
// (parallel). Otherwise,
//   [a  b]T = inv([q - p  r - s])*[r - p]
//
// The line segments have intersection iif 0 < a < 1 and 0 < b < 1, excluding
// the intersection at the ends.
enum class LineType { segment, ray, infinite_line };

bool
does_intersect (
    const Point&   p,
    const Point&   q,
    const Point&   r,
    const Point&   s,
    const LineType line_type        = LineType::segment,
    const bool     ignore_endpoints = true
);

// Calculate the area of a triangle
double
area (const Point& a, const Point& b, const Point& c);

// Mid-point between two points
Point
midpoint (const Point& a, const Point& b);

}  // namespace geometry

#endif
