#ifndef COORDINATE_HPP
#define COORDINATE_HPP

#include <ostream>
#include <utility>

class Coordinate {
 public:
  double x, y, z;
  Coordinate();
  Coordinate(double x, double y, double z);
  Coordinate(double scalar);
  Coordinate(const Coordinate &other);
  Coordinate(Coordinate &&other);
  Coordinate &operator=(const Coordinate &other);
  Coordinate &operator=(Coordinate &&other);
  Coordinate operator+(const Coordinate &other) const;
  Coordinate operator+(const double scalar) const;
  Coordinate operator-(const Coordinate &other) const;
  Coordinate operator-() const;
  Coordinate operator-(const double scalar) const;
  Coordinate operator*(const Coordinate &other) const;
  Coordinate operator*(const double scalar) const;
  Coordinate operator/(const Coordinate &other) const;
  Coordinate operator/(const double scalar) const;
  friend std::ostream &operator<<(std::ostream &stream, const Coordinate &c);

  double dot(const Coordinate &other) const;
  Coordinate cross(const Coordinate &other) const;
  Coordinate normalize() const;
  double dist() const;
};

#endif
