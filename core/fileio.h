//
// fileio.h
//
// Definitions for file and stream input/output
//
#ifndef __FILE_IO_H__
#define __FILE_IO_H__

#include <string>
#include <vector>

#include "core/types.h"

//// NAMESPACE: fileio

namespace fileio {

// Read CSV file into a vector.
std::vector<Point>
read_csv_points (const std::string& filename);

// Write TeX with TikZ routine to draw polygon and triangles.
std::string
string_tikz_polygon (
    const Points&            points,
    const std::vector<bool>& clipped,
    const Triangles&         triangles,
    const double             scale
);
void
write_tex_tikz (
    const std::string& filename,
    const Points&      points,
    const Triangles&   triangles,
    const double       area,
    const double       scale
);

// Convert a Point to stream
std::ostream&
operator<< (std::ostream& os, const Point& p);

// Convert a Polar to stream
std::ostream&
operator<< (std::ostream& os, const Polar& p);

}  // namespace fileio

#endif
