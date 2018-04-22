#include "coordinate.hpp"
#include <cmath>

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

std::ostream &operator<<(std::ostream &stream, const Coordinate &c) {
  stream << "x: " << c.x << " y: " << c.y << " z: " << c.z;
  return stream;
}

double Coordinate::dot(const Coordinate &other) const {
  return x * other.x + y * other.y + z * other.z;
}

Coordinate Coordinate::cross(const Coordinate &other) const {
  return Coordinate(y * other.z - z * other.y, z * other.x - x * other.z,
                    x * other.y - y * other.x);
}

Coordinate Coordinate::normalize() const { return *this / dist(); }

double Coordinate::dist() const {
  return std::sqrt(std::pow(x, 2.0) + std::pow(y, 2.0) + std::pow(z, 2.0));
}