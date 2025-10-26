#include "core/polygon.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numbers>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "core/fileio.h"
#include "core/geometry.h"
#include "core/numeric.h"

Polygon::Polygon(Points&& pts) : _points{pts} {
  _interior_dir = determine_interior_direction(_points);
}

Polygon::~Polygon() {
  if (_debug_tex_tikz_file.is_open()) _debug_tex_tikz_file.close();
}

// Triangulate using ear clipping algorithm
Triangles Polygon::triangulate() const {
#define __DEBUG_TIKZ__
#ifdef __DEBUG_TIKZ__
  open_debug_tikz("debug.tex");
#endif

  // If the interior cannot be determined, return empty result.
  if (_interior_dir == InteriorDirection::unknown) return Triangles{};

  Triangles triangles;

  // Create a vector to mark whether a vertex is clipped.
  std::vector<bool> clipped(_points.size() - 1, false);

  std::size_t current_idx = 0;
  _area = 0.0;

  while (std::count(clipped.cbegin(), clipped.cend(), false) > 3) {
    ThreeIndices indices = three_unclipped_vertices(clipped, current_idx);

    // Naming convention:
    //  vp: vertex_previous
    //  v : current vertex
    //  vn: vertex_next
    const std::size_t idx_vp = std::get<0>(indices);
    const std::size_t idx_v = std::get<1>(indices);
    const std::size_t idx_vn = std::get<2>(indices);

    const Point vp = _points[idx_vp];
    const Point v = _points[idx_v];
    const Point vn = _points[idx_vn];

    std::cout << "[" << current_idx << "] " << idx_vp << ", " << idx_v << ", "
              << idx_vn << " : ";

    // Check two conditions whether the three points vp, v, and vn
    // form a triangle "inside" the polygon.

    // 1. Does the line segment (vp, vn) lie inside the polygon?
    const double q_1 = geometry::angle(vp, v);
    const double q_2 = geometry::angle(v, vn);
    double q_diff = q_2 - q_1;
    if (q_diff > std::numbers::pi) q_diff -= -2 * std::numbers::pi;
    if (q_diff < -std::numbers::pi) q_diff += 2 * std::numbers::pi;

    if ((q_diff > 0. && _interior_dir != InteriorDirection::left) ||
        (q_diff < 0. && _interior_dir != InteriorDirection::right) ||
        numeric::close_enough(q_diff, 0.)) {
      // The line segment connecting vp to vn is OUTSIDE of the polygon.
      std::cout << "o" << std::endl;
      increase_idx(current_idx);
      continue;
    }

    if (numeric::close_enough(q_diff, std::numbers::pi) ||
        numeric::close_enough(q_diff, -std::numbers::pi)) {
      // The three points vp, v, and vn form a degenerate feature,
      // which means the line segments (vp, v) and (v, vn) overlap.
      std::cout << "x" << std::endl;
      clipped[idx_v] = true;
      increase_idx(current_idx);
      continue;
    }

    // 2. Is the line segment (vp, vn) free from intersection with other line
    // segments in the polygon?
    //
    // NOTE: This is the most inefficient part of this function.
    if (has_intersection(clipped, idx_vp, idx_v, idx_vn)) {
      increase_idx(current_idx);
      continue;
    }

    // Register a triangle
    register_triangle(triangles, idx_vp, idx_v, idx_vn);
    _area += geometry::area(_points[idx_vp], _points[idx_v], _points[idx_vn]);

#ifdef __DEBUG_TIKZ__
    append_debug_tikz(clipped, triangles);
#endif

    // Remove the point p and begin with a new head.
    clipped[idx_v] = true;
    std::cout << "*" << std::endl;
  }

  // Add the remaining triangle
  ThreeIndices indices = three_unclipped_vertices(clipped, current_idx);

  const std::size_t idx_vp = std::get<0>(indices);
  const std::size_t idx_v = std::get<1>(indices);
  const std::size_t idx_vn = std::get<2>(indices);

  std::cout << idx_vp << ", " << idx_v << ", " << idx_vn << " : ";
  register_triangle(triangles, idx_vp, idx_v, idx_vn);
  _area += geometry::area(_points[idx_vp], _points[idx_v], _points[idx_vn]);
  std::cout << "*" << std::endl;

#ifdef __DEBUG_TIKZ__
  append_debug_tikz(clipped, triangles);
  close_debug_tikz();
#endif

  return triangles;
}

double Polygon::area() const { return _area; }

void Polygon::increase_idx(std::size_t& idx) const {
  idx = (idx + 1) % (_points.size() - 1);
}

// Distinguish interior and outside of polygon
// Assume that the points form a closed polygon, i.e., the first and last
// elements coincide.
Polygon::InteriorDirection Polygon::determine_interior_direction(
    const Points& points) const {
  std::vector<double> angles(points.size() - 1, 0.0);

  for (std::size_t i = 0; i < points.size() - 1; ++i) {
    angles[i] = geometry::angle(points[i], points[i + 1]);
  }

  double sum_rotational_angles = 0.0;
  for (std::size_t i = 0; i < angles.size() - 1; ++i) {
    sum_rotational_angles +=
        geometry::constrain_rotational_angle(angles[i + 1] - angles[i]);
  }
  sum_rotational_angles +=
      geometry::constrain_rotational_angle(angles.front() - angles.back());

  if (numeric::close_enough(sum_rotational_angles, 2 * std::numbers::pi))
    return InteriorDirection::left;
  if (numeric::close_enough(sum_rotational_angles, -2 * std::numbers::pi))
    return InteriorDirection::right;

  return InteriorDirection::unknown;
}

void Polygon::register_triangle(Triangles& triangles, const std::size_t a,
                                const std::size_t b,
                                const std::size_t c) const {
  // Register a triangle and calculate area.
  // Arrange the vertices so that the normal of the triangle is aligned
  // with the surface (or the plane which contains the polygon) normal.
  if (_interior_dir == InteriorDirection::right) {
    triangles.push_back(TriangleSpec{a, c, b});
  } else {  // InteriorDirection::left
    triangles.push_back(TriangleSpec{a, b, c});
  }
}

// Find the index to a vertex which is not clipped, from offset.
std::optional<std::size_t> Polygon::index_unclipped_vertex(
    const std::vector<bool>& clipped, const std::size_t offset) const {
  if (offset > clipped.size() - 1) return std::nullopt;

  auto it = std::find(clipped.cbegin() + offset, clipped.cend(), false);
  if (it == clipped.cend()) {  // Not found from offset to the end
    if (offset != 0) {
      // Find from begin to offset
      it = std::find(clipped.cbegin(), clipped.cbegin() + offset, false);
      if (it == clipped.cbegin() + offset)  // Still not found
        return std::nullopt;
      else
        return std::distance(clipped.cbegin(), it);
    } else  // Not found from begin to end
      return std::nullopt;
  }
  return std::distance(clipped.cbegin(), it);
}

// Find three consecutive vertices which is not clipped, beginning from offset.
// If idx is at either of the end of the vector, this function finds neighbors
// regarding the vector as a circular buffer.
//
// NOTE: when this function returns (0, 0, 0), it means it failed to find the
// vertices.
Polygon::ThreeIndices Polygon::three_unclipped_vertices(
    const std::vector<bool>& clipped, const std::size_t offset) const {
  // First, check whether there are three unclipped vertices
  if (std::count(clipped.cbegin(), clipped.cend(), false) < 3)
    return std::make_tuple(0, 0, 0);  // This indicates NOT found

  // Searching from offset
  std::size_t idx1, idx2, idx3;

  auto opt_idx1 = index_unclipped_vertex(clipped, offset);
  if (opt_idx1.has_value())
    idx1 = opt_idx1.value();
  else
    idx1 = 0;

  auto opt_idx2 = index_unclipped_vertex(clipped, idx1 + 1);
  if (opt_idx2.has_value())
    idx2 = opt_idx2.value();
  else
    idx2 = 0;

  auto opt_idx3 = index_unclipped_vertex(clipped, idx2 + 1);
  if (opt_idx3.has_value())
    idx3 = opt_idx3.value();
  else
    idx3 = 0;

  return std::make_tuple(idx1, idx2, idx3);
}

bool Polygon::has_intersection(const std::vector<bool>& clipped,
                               const std::size_t idx_vp,
                               const std::size_t idx_v,
                               const std::size_t idx_vn) const {
  for (std::size_t i = 0; i < _points.size(); ++i) {
    const auto opt_i = index_unclipped_vertex(clipped, i);
    if (!opt_i.has_value()) break;

    const std::size_t idx_seg_i = opt_i.value();

    const auto opt_j = index_unclipped_vertex(clipped, idx_seg_i + 1);
    if (!opt_j.has_value()) break;

    const std::size_t idx_seg_j = opt_j.value();

    // Exclude line segments which coincide with the current vertices.
    if ((idx_seg_j == idx_vp) || (idx_seg_i == idx_vp && idx_seg_j == idx_v) ||
        (idx_seg_i == idx_v && idx_seg_j == idx_vn) || idx_seg_i == idx_vn)
      continue;

    const Point r = _points[idx_seg_i];
    const Point s = _points[idx_seg_j];

    if (geometry::does_intersect(_points[idx_vp], _points[idx_vn], r, s)) {
      std::cout << "+ " << idx_v << " - " << idx_seg_i << ", " << idx_seg_j
                << std::endl;
      return true;
    }
  }
  return false;
}

std::string Polygon::interior_direction() const {
  switch (_interior_dir) {
    case InteriorDirection::left:
      return "left";
      break;
    case InteriorDirection::right:
      return "right";
      break;
    default:
      return "unknown";
      break;
  }
}

void Polygon::open_debug_tikz(const std::string& filename) const {
  _debug_tex_tikz_file = std::ofstream{filename};

  if (!_debug_tex_tikz_file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }
  _debug_tex_tikz_file << "\\input tikz.tex\n"
                       << "\\baselineskip=12pt\n"
                       << "\\hsize=6.3truein\n"
                       << "\\vsize=8.7truein\n"
                       << "\\nopagenumbers\n";
}

void Polygon::close_debug_tikz() const {
  _debug_tex_tikz_file << "\\vfill\\eject\n" << "\\bye\n";
  _debug_tex_tikz_file.close();
}

void Polygon::append_debug_tikz(const std::vector<bool>& flags,
                                const Triangles& triangles) const {
  _debug_tex_tikz_file << "Triangulation:\n"
                       << "\\vskip12pt\n";

  _debug_tex_tikz_file << fileio::string_tikz_polygon(_points, flags, triangles,
                                                      0.25);

  _debug_tex_tikz_file << "\\vfill\\eject\n";
}
