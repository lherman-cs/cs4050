#ifndef UTILS_HPP
#define UTILS_HPP

#include <unordered_map>
#include <vector>
#include "color.hpp"
#include "coordinate.hpp"
#include "model.hpp"
#include "triangle.hpp"

bool find_closest_intersection(
    const std::vector<Triangle> &triangles,
    std::unordered_map<Coordinate *, Coordinate> &normals,
    const Coordinate &eye, const Coordinate &direction, Triangle *closest,
    Coordinate *intersected_point, Coordinate *normal);
bool is_shadowed(const std::vector<Triangle> &triangles, const Coordinate &eye,
                 const Coordinate &direction, const Triangle &surface);

Color get_diffuse(const Coordinate &to_source, const Coordinate &normal);
Color get_specular(const Coordinate &to_source, const Coordinate &normal,
                   const Coordinate &to_viewer);
Color get_ambient();
#endif
