#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <float.h>
#include <limits.h>
#include "coordinate.hpp"
#include "face.hpp"

class Triangle {
 public:
  static constexpr double NOT_INTERSECTED = -DBL_MAX;
  Coordinate *vertices[3];

  Triangle(Coordinate *a, Coordinate *b, Coordinate *c);
  Triangle(Face &face);
  Triangle();
  double intersects(const Coordinate &eye, const Coordinate &direction) const;
  bool is_empty() const;
  Coordinate normal() const;
};

#endif