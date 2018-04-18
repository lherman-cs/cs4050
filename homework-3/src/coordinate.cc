#include "coordinate.hpp"
#include <math.h>

Coordinate::Coordinate() : x(0.0), y(0.0), z(0.0) {}

Coordinate::Coordinate(double x, double y, double z) : x(x), y(y), z(z) {}

Coordinate::Coordinate(double scalar) : x(scalar), y(scalar), z(scalar) {}

Coordinate::Coordinate(const Coordinate &other) {
  x = other.x;
  y = other.y;
  z = other.z;
}

Coordinate::Coordinate(Coordinate &&other) {
  x = std::move(other.x);
  y = std::move(other.y);
  z = std::move(other.z);
}

Coordinate &Coordinate::operator=(const Coordinate &other) {
  x = other.x;
  y = other.y;
  z = other.z;
  return *this;
}

Coordinate &Coordinate::operator=(Coordinate &&other) {
  x = std::move(other.x);
  y = std::move(other.y);
  z = std::move(other.z);
  return *this;
}

Coordinate Coordinate::operator+(const Coordinate &other) const {
  return {x + other.x, y + other.y, z + other.z};
}

Coordinate Coordinate::operator+(const double scalar) const {
  return {x + scalar, y + scalar, z + scalar};
}

Coordinate Coordinate::operator-(const Coordinate &other) const {
  return {x - other.x, y - other.y, z - other.z};
}

Coordinate Coordinate::operator-() const { return {-x, -y, -z}; }

Coordinate Coordinate::operator-(const double scalar) const {
  return {x - scalar, y - scalar, z - scalar};
}

Coordinate Coordinate::operator*(const Coordinate &other) const {
  return {x * other.x, y * other.y, z * other.z};
}

Coordinate Coordinate::operator*(const double scalar) const {
  return {x * scalar, y * scalar, z * scalar};
}

Coordinate Coordinate::operator/(const Coordinate &other) const {
  return {x / other.x, y / other.y, z / other.z};
}

Coordinate Coordinate::operator/(const double scalar) const {
  return {x / scalar, y / scalar, z / scalar};
}

double Coordinate::dot(const Coordinate &other) const {
  return x * other.x + y * other.y + z * other.z;
}

Coordinate Coordinate::cross(const Coordinate &other) const {
  return Coordinate(y * other.z - z * other.y, z * other.x - x * other.z,
                    x * other.y - y * other.x);
}

Coordinate Coordinate::normalize() const {
  double distance = std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2);
  distance = std::sqrt(distance);

  return *this / distance;
}