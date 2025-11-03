#include <algorithm>
#include <numbers>
#include <ranges>
#include <string>

#include "core/fileio.h"
#include "core/geometry.h"
#include "core/random.h"

// Place points around a circle
std::vector<Point>
place_points_around_circle (const double radius, const int count_points) {
    const double        inc = 2. * std::numbers::pi / count_points;
    std::vector<double> angles (count_points);
    for (int i : std::views::iota (0, count_points)) {
        angles[i] = i * inc;
    }

    std::vector<Point> points (count_points);
    std::transform (angles.cbegin(), angles.cend(), points.begin(), [radius] (const double angle) {
        return Point{radius * std::cos (angle), radius * std::sin (angle)};
    });

    return points;
}

// Check self-intersection
bool
has_self_intersection (
    const Point&              p_pre,
    const Point&              p,
    const Point&              p_nxt,
    const std::size_t         i,
    const std::vector<Point>& points
) {
    for (std::size_t j = 0; j < points.size(); ++j) {
        const Point r = points[j];
        const Point s = points[(j + 1) % points.size()];
        if ((j + 1) % points.size() == i || j == i) {
            if (geometry::does_intersect (p_pre, p, r, s) ||
                geometry::does_intersect (p, p_nxt, r, s)) {
                return true;
            }
        } else if (geometry::does_intersect (p_pre, p, r, s, geometry::LineType::segment, false) ||
                   geometry::does_intersect (p, p_nxt, r, s, geometry::LineType::segment, false)) {
            return true;
        }
    }
    return false;
}

// Move points randomly without creating intersections
void
move_points_randomly (
    std::vector<Point>& points,
    const double        max_distance,
    const int           count_movements = 1
) {
    random_float_gen<double> random;

    for ([[maybe_unused]] std::size_t _ : std::views::iota (0, count_movements)) {
        for (std::size_t i = 0; i < points.size(); ++i) {
            const double angle = random() * 2. * std::numbers::pi;
            const double dist  = random() * max_distance;

            const Point movement{dist * std::cos (angle), dist * std::sin (angle)};
            const Point p_moved = points[i] + movement;
            const Point p_pre   = points[i == 0 ? points.size() - 1 : i - 1];
            const Point p_nxt   = points[(i + 1) % points.size()];

            if (has_self_intersection (p_pre, p_moved, p_nxt, i, points)) continue;

            points[i] = p_moved;
        }
    }
}

// Generate random polygon
void
generate (const std::size_t count_points, const std::string filename) {
    // Create random points
    std::vector<Point> points = place_points_around_circle (50., count_points);
    move_points_randomly (points, 10., 40);

    const std::string ext{".csv"};
    fileio::write_points_csv_file (points, "polygons/" + filename + ".csv", true);
}

//// main
int
main (int argc, char* argv[]) {
    generate (40, "random_polygon_0");
}
