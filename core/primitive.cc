#include "primitive.h"

#include <cmath>

Point
operator+ (const Point& a, const Point& b) {
    return Point{a.x + b.x, a.y + b.y};
}

bool
operator== (const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}

bool
close_enough (const Point& a, const Point& b, const double threshold) {
    return std::abs (a.x - b.x) < threshold && std::abs (a.y - b.y) < threshold;
}
