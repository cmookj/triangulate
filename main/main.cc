#include <iostream>
#include <string>

#include "core/fileio.h"
#include "core/polygon.h"

// Triangulate
void
triangulate (const std::string& dir, std::string filename_ext, const double scale) {
    auto    points = fileio::read_csv_points (dir + "/" + filename_ext);
    Polygon poly{std::move (points)};

    Triangles    triangles = poly.triangulate();
    const double area      = poly.area();

    std::cout << "Area = " << area << '\n';

    const std::string ext{".csv"};
    std::string       filename = filename_ext.erase (filename_ext.size() - ext.size());
    fileio::write_tex_tikz ("examples/output/" + filename + ".tex", points, triangles, area, scale);
}

//// main
int
main (int argc, char* argv[]) {
    triangulate ("examples", "regular_polygon_10.csv", 1.0);
    triangulate ("examples", "regular_polygon_20.csv", 1.0);
    triangulate ("examples", "regular_polygon_40.csv", 1.0);
    triangulate ("examples", "regular_polygon_80.csv", 1.0);
    triangulate ("examples", "regular_polygon_160.csv", 1.0);
    triangulate ("examples", "regular_polygon_320.csv", 1.0);
    triangulate ("examples", "regular_polygon_640.csv", 1.0);
    triangulate ("examples", "regular_polygon_1280.csv", 1.0);

    triangulate ("examples", "example_0.csv", 10);
    triangulate ("examples", "example_1.csv", 5);
    triangulate ("examples", "example_2.csv", 0.25);
}
