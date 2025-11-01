#include "types.h"

Point
operator+ (const Point& a, const Point& b) {
    return Point{a.x + b.x, a.y + b.y};
}
