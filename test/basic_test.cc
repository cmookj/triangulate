#include <gtest/gtest.h>

#include <cmath>
#include <numbers>

#include "test/random_gen.h"

#include "core/geometry.h"
#include "core/numeric.h"
#include "core/polygon.h"
#include "core/types.h"

//// For a limited form of QuickCheck
constexpr std::size_t max_test_count = 65536;

//// Test Numeric
TEST (NumericTest, CloseEnough) {
    random_float_gen<double> gen;
    const double             eps = 1e-12;

    for (std::size_t i = 0; i < max_test_count; ++i) {
        double rn = gen();
        ASSERT_TRUE (numeric::close_enough (rn, rn));
        ASSERT_FALSE (numeric::close_enough (rn, rn + 2 * eps, eps));
    }
}

//// Test Geometry
TEST (GeometryTest, CartesianPolarConversion) {
    random_float_gen<double> gen;

    for (std::size_t i = 0; i < max_test_count; ++i) {
        double r = 1.0;
        double q = gen();

        double x = r * std::cos (q);
        double y = r * std::sin (q);

        auto polar = geometry::polar_coordinate (Point{0., 0.}, Point{x, y});
        EXPECT_NEAR (polar.length, 1.0, 1e-12);
        EXPECT_NEAR (polar.angle, q, 1e-12);
    }
}

TEST (GeometryTest, Area) {
    // Area of a random triangle whose two vertices are fixed at (0, 0) and (1, 0).
    // The other vertex is randomly picked on the line y = 2.
    random_float_gen<double> gen;

    Point a{0., 0.};
    Point b{1., 0.};

    for (std::size_t i = 0; i < max_test_count; ++i) {
        Point        c{gen(), 2.};
        const double area = geometry::area (a, b, c);
        EXPECT_NEAR (area, 1.0, 1e-12);
    }
}

TEST (GeometryTest, Intersection) {
    // Four points are picked randomly such that
    // each of them lie on each edge of a unit square.
    //
    // For example,
    //
    //     ^
    //     |        c
    //     +--------.---+
    //     |            |
    //     |            .b
    //     |            |
    //    a.            |
    //     |            |
    //     +-----.------+---->
    //           d
    // Then two line segments (a, d) and (b, c) should always have an intersection.
    random_float_gen<double> gen;

    Point a{0., gen()};
    Point b{1., gen()};
    Point c{gen(), 1.};
    Point d{gen(), 0.};

    for (std::size_t i = 0; i < max_test_count; ++i) {
        ASSERT_TRUE (geometry::does_intersect (a, b, c, d));
    }
}

//// Polygon
TEST (PolygonTest, InteriorDirection) {
    // Create a perimeter of a circle in CCW direction
    constexpr std::size_t count_vertices = 12;
    const double          inc = 2. * std::numbers::pi / static_cast<double> (count_vertices);

    std::vector<Point> points_ccw;
    for (std::size_t i = 0; i < count_vertices; ++i) {
        const double q = static_cast<double> (i) * inc;
        const double x = std::cos (q);
        const double y = std::sin (q);
        points_ccw.push_back (Point{x, y});
    }
    Polygon poly_ccw{std::move (points_ccw)};
    EXPECT_EQ (poly_ccw.interior_direction(), "left");

    // Create a perimeter of a circle in CW direction
    std::vector<Point> points_cw;
    for (std::size_t i = 0; i < count_vertices; ++i) {
        const double q = -static_cast<double> (i) * inc;
        const double x = std::cos (q);
        const double y = std::sin (q);
        points_cw.push_back (Point{x, y});
    }
    Polygon poly_cw{std::move (points_cw)};
    EXPECT_EQ (poly_cw.interior_direction(), "right");
}
