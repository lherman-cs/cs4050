#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <float.h>
#include <limits.h>
#include "coordinate.hpp"

class Triangle {
 public:
  static constexpr double NOT_INTERSECTED = -DBL_MAX;
  Coordinate *vertices[3];

  Triangle(Coordinate *a, Coordinate *b, Coordinate *c);
  Triangle();
  bool operator==(const Triangle &other) const;
  bool operator!=(const Triangle &other) const;
  double intersects(const Coordinate &eye, const Coordinate &direction) const;
  bool is_in(const Coordinate &point) const;
  bool is_empty() const;
  Coordinate normal() const;
};

#endif