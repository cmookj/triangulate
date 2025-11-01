#include "core/geometry.h"

#include <cmath>
#include <numbers>

#include "core/numeric.h"

//// NAMESPACE: geometry
namespace geometry {

// Distance between two points
double
distance (const Point& p, const Point& q) {
    return std::sqrt (std::pow (p.x - q.x, 2) + std::pow (p.y - q.y, 2));
}

// Angle of a line segment w.r.t. x-axis
double
angle (const Point& p, const Point& q) {
    return std::atan2 (q.y - p.y, q.x - p.x);
}

// Calculates polar coordinate representation of a line segment
// defined by two points.
Polar
polar_coordinate (const Point& p, const Point& q) {
    return Polar{.length = distance (p, q), .angle = angle (p, q)};
}

// Constrain the angle of rotation between -180 and 180
double
constrain_rotational_angle (const double q) {
    if (q > std::numbers::pi) return q - 2 * std::numbers::pi;
    if (q < -std::numbers::pi) return q + 2 * std::numbers::pi;
    return q;
}

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
bool
does_intersect (
    const Point&   p,
    const Point&   q,
    const Point&   r,
    const Point&   s,
    const LineType line_type,
    const bool     ignore_endpoints
) {
    const double A_11 = q.x - p.x;
    const double A_12 = r.x - s.x;
    const double A_21 = q.y - p.y;
    const double A_22 = r.y - s.y;

    const double det = A_11 * A_22 - A_12 * A_21;
    if (numeric::close_enough (det, 0.0)) return false;

    const double b_1 = r.x - p.x;
    const double b_2 = r.y - p.y;

    const double a = (A_22 * b_1 - A_12 * b_2) / det;
    const double b = (-A_21 * b_1 + A_11 * b_2) / det;

    if (ignore_endpoints) {
        switch (line_type) {
        case LineType::segment: return (0. < a && a < 1. && 0. < b && b < 1.); break;
        case LineType::ray: return (0. < a && 0. < b && b < 1.); break;
        case LineType::infinite_line: return (0. < b && b < 1.); break;
        }
    } else {
        switch (line_type) {
        case LineType::segment: return (0. <= a && a <= 1. && 0. <= b && b <= 1.); break;
        case LineType::ray: return (0. <= a && 0. <= b && b <= 1.); break;
        case LineType::infinite_line: return (0. <= b && b <= 1.); break;
        }
    }

    return false;
}

// Calculate the area of a triangle
double
area (const Point& a, const Point& b, const Point& c) {
    const double px = b.x - a.x;
    const double py = b.y - a.y;

    const double qx = c.x - a.x;
    const double qy = c.y - a.y;

    return .5 * std::abs (px * qy - py * qx);
}

// Mid-point between two points
Point
midpoint (const Point& a, const Point& b) {
    return Point{(a.x + b.x) / 2., (a.y + b.y) / 2.};
}

}  // namespace geometry
