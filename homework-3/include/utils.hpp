#ifndef UTILS_HPP
#define UTILS_HPP

#include <unordered_map>
#include <vector>
#include "color.hpp"
#include "coordinate.hpp"
#include "face.hpp"
#include "model.hpp"
#include "texture.hpp"
#include "triangle.hpp"

enum class Axis { x, y };

Coordinate get_avg(std::vector<Coordinate> &vertices);
Coordinate get_max(std::vector<Coordinate> &vertices);
Coordinate get_min(std::vector<Coordinate> &vertices);
void normalize(std::vector<Coordinate> &vertices, double height, double width,
               double scale);
void move(std::vector<Coordinate> &vertices, const Coordinate &to);
void scale(std::vector<Coordinate> &vertices, double factor);
void rotate(std::vector<Coordinate> &vertices, Axis axis, double degree);

bool find_closest_intersection(const std::vector<Face> &faces,
                               const Coordinate &eye,
                               const Coordinate &direction, Triangle *closest,
                               Coordinate *intersected_point);

Color get_diffuse(const Coordinate &to_source, const Coordinate &normal);
Color get_specular(const Coordinate &to_source, const Coordinate &normal,
                   const Coordinate &to_viewer);
Color get_ambient();
#endif
