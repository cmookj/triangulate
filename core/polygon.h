//
// polygon.h
//
// polygon class to triangulate a polygon
//

#ifndef __POLYGON_H__
#define __POLYGON_H__

#include <fstream>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "core/types.h"

//// class Polygon

class Polygon {
    using ThreeIndices = std::tuple<std::size_t, std::size_t, std::size_t>;

  public:
    // .left: the left side of line segments is the interior of a polygon
    // .right: the right side of line segments is the interior of a polygon
    enum class InteriorDirection { left, right, unknown };

    Polygon (Points&& pts);

    ~Polygon ();

    // Triangulate using ear clipping algorithm
    Triangles
    triangulate () const;

    double
    area () const;

    std::string
    interior_direction () const;

  private:
    void
    increase_idx (std::size_t& idx) const;

    // Distinguish interior and outside of polygon
    // Assume that the points form a closed polygon, i.e., the first and last
    // elements coincide.
    void
    determine_interior_direction (const Points& points) const;

    void
    register_triangle (
        Triangles&        triangles,
        const std::size_t a,
        const std::size_t b,
        const std::size_t c
    ) const;

    // Find the index to a vertex which is not clipped, from offset.
    std::optional<std::size_t>
    index_unclipped_vertex (const std::vector<bool>& clipped, const std::size_t offset = 0) const;

    // Find three consecutive vertices which is not clipped, beginning from
    // offset. If idx is at either of the end of the vector, this function finds
    // neighbors regarding the vector as a circular buffer.
    //
    // NOTE: when this function returns (0, 0, 0), it means it failed to find the
    // vertices.
    ThreeIndices
    three_unclipped_vertices (const std::vector<bool>& clipped, const std::size_t offset = 0) const;

    bool
    has_intersection (
        const std::vector<bool>& clipped,
        const std::size_t        idx_vp,
        const std::size_t        idx_v,
        const std::size_t        idx_vn
    ) const;

    void
    open_debug_tikz (const std::string& filename) const;

    void
    close_debug_tikz () const;

    void
    append_debug_tikz (const std::vector<bool>& flags, const Triangles& triangles) const;

  private:
    Points                    _points;
    mutable double            _area;
    mutable InteriorDirection _interior_dir;
    mutable std::ofstream     _debug_tex_tikz_file;
};

#endif
