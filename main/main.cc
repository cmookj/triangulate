#include "core/fileio.h"
#include "core/polygon.h"

#include <iostream>
#include <string>

// Triangulate
void
triangulate (const std::string& dir, const std::string& filename, const double scale) {
    auto    points = fileio::read_csv_points (dir + "/" + filename);
    Polygon poly{std::move (points)};

    Triangles    triangles = poly.triangulate();
    const double area      = poly.area();

    std::cout << "Area = " << area << '\n';

    fileio::write_tex_tikz (filename + ".tex", points, triangles, area, scale);
}

//// main
int
main (int argc, char* argv[]) {
    triangulate ("data", "simple_concave_poly.csv", 10);
    triangulate ("data", "concave_poly.csv", 0.25);
}
