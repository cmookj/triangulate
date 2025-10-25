#include "core/fileio.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

//// NAMESPACE: fileio

namespace fileio {

// Read CSV file into a vector.
std::vector<Point>
read_csv_points (const std::string& filename) {
    std::vector<Point> points;
    std::ifstream      file (filename);

    if (!file.is_open()) {
        throw std::runtime_error ("Failed to open file: " + filename);
    }

    std::string line;
    while (std::getline (file, line)) {
        if (line.empty()) continue;

        std::istringstream ss (line);
        std::string        x_str, y_str;

        if (!std::getline (ss, x_str, ',')) continue;
        if (!std::getline (ss, y_str, ',')) continue;

        Point p;
        p.x = std::stod (x_str);
        p.y = std::stod (y_str);
        points.push_back (p);
    }

    return points;
}

// Write TeX with TikZ routine to draw polygon and triangles.
std::string
string_tikz_polygon (
    const Points&            points,
    const std::vector<bool>& clipped,
    const Triangles&         triangles,
    const double             scale
) {
    std::stringstream strm;

    // Create temporary points
    Points points_enabled;
    points_enabled.reserve (std::count (clipped.cbegin(), clipped.cend(), true));

    for (size_t i = 0; i < points.size(); ++i) {
        if (clipped[i] == false) points_enabled.push_back (points[i]);
    }

    strm << "\\tikzpicture[scale=" << scale << "]\n";
    strm << "\\draw[thick]\n";
    for (std::size_t i = 0; i < points_enabled.size() - 1; ++i) {
        strm << points_enabled[i] << " -- \n";
    }
    strm << points_enabled.back() << ";\n";

    for (const auto& tri : triangles) {
        strm << "\\draw[ultra thin]" << points[tri[0]] << " -- " << points[tri[1]] << " -- "
             << points[tri[2]] << " -- cycle;\n";
    }
    strm << "\\endtikzpicture\n";

    return strm.str();
}

void
write_tex_tikz (
    const std::string& filename,
    const Points&      points,
    const Triangles&   triangles,
    const double       area,
    const double       scale
) {
    std::ofstream file (filename);

    if (!file.is_open()) {
        throw std::runtime_error ("Failed to open file: " + filename);
    }

    file << "\\input tikz.tex\n"
         << "\\baselineskip=12pt\n"
         << "\\hsize=6.3truein\n"
         << "\\vsize=8.7truein\n";

    file << "The original polygon:\n"
         << "\\vskip12pt\n";

    // Original polygon
    file << "\\tikzpicture[scale=" << scale << "]\n";
    file << "\\draw[thick]\n";
    for (std::size_t i = 0; i < points.size() - 1; ++i) {
        file << points[i] << " -- \n";
    }
    file << points.back() << ";\n";

    file << "\\endtikzpicture\n"
         << "\\vfill\\eject\n";

    file << "Triangulation:\n"
         << "\\vskip12pt\n";

    // Triangulation
    file << "\\tikzpicture[scale=" << scale << "]\n";
    // Outer polygon
    file << "\\draw[thick]\n";
    for (std::size_t i = 0; i < points.size() - 1; ++i) {
        file << points[i] << " -- \n";
    }
    file << points.back() << ";\n";

    for (const auto& tri : triangles) {
        file << "\\draw[ultra thin]" << points[tri[0]] << " -- " << points[tri[1]] << " -- "
             << points[tri[2]] << " -- cycle;\n";
    }
    file << "\\endtikzpicture\n";

    file << "\\vskip24pt\n"
         << "Area = " << area << "\n";

    file << "\\vfill\\eject\n"
         << "\\bye\n";
}

// Convert a Point to stream
std::ostream&
operator<< (std::ostream& os, const Point& p) {
    return os << "(" << p.x << ", " << p.y << ")";
}

// Convert a Polar to stream
std::ostream&
operator<< (std::ostream& os, const Polar& p) {
    return os << "(" << p.length << ", " << p.angle << ")";
}

}  // namespace fileio
