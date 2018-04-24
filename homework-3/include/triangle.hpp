#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <ostream>
#include "coordinate.hpp"

class Triangle {
 public:
  Coordinate *vertices[3];
  Coordinate normal;

  Triangle(Coordinate *a, Coordinate *b, Coordinate *c);
  Triangle();

  Triangle(const Triangle &other);
  Triangle(Triangle &&other);
  Triangle &operator=(const Triangle &other);
  Triangle &operator=(Triangle &&other);
  bool operator==(const Triangle &other) const;
  bool operator!=(const Triangle &other) const;
  friend std::ostream &operator<<(std::ostream &stream, const Triangle &tri);
  bool intersects(const Coordinate &eye, const Coordinate &direction,
                  double *t) const;
  bool intersects(const Coordinate &eye, const Coordinate &direction, double *t,
                  double *beta, double *gamma) const;
  bool is_in(const Coordinate &point) const;
  bool is_empty() const;

 private:
  Coordinate get_normal() const;
};

#endif